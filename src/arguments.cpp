#include <native.hpp>

exo::Object character_to_binary(char const character, exo::Context const& context) {
    auto const&& bit_library { exo::find_parent("bit", context) };
    auto const&& true_bit { exo::get_property("true", bit_library, context) };
    auto const&& false_bit { exo::get_property("false", bit_library, context) };

    auto const&& stack_library { exo::find_parent("stack", context) };
    auto const&& empty_stack { exo::get_property("empty", stack_library, context) };

    auto const&& binary_library { exo::find_parent("binary", context) };
    auto const&& binary_constructor { exo::get_property("of", binary_library, context) };

    auto bit_stack { empty_stack };

    for (uint8_t character_index { 8 }; character_index > 0; --character_index) {
        auto const&& index_is_one { character & (0b1 << (character_index - 1)) };
        auto const& bit_value { index_is_one ? true_bit : false_bit };

        auto const&& push_function { exo::get_property("push", bit_stack, context) };
        bit_stack = exo::call(bit_value, push_function, context);
    }

    return exo::call(bit_stack, binary_constructor, context);
}

exo::Object c_string_to_string(char const* character_address, exo::Context const& context) {
    auto const&& stack_library { exo::find_parent("stack", context) };
    auto const&& empty_stack { exo::get_property("empty", stack_library, context) };

    auto const&& string_library { exo::find_parent("string", context) };
    auto const&& string_constructor { exo::get_property("of", string_library, context) };

    auto character_stack { empty_stack };

    while (char const character = *character_address) {
        auto const&& binary_character { character_to_binary(character, context) };
        auto const&& push_function { exo::get_property("push", character_stack, context) };
        character_stack = exo::call(binary_character, push_function, context);

        character_address += sizeof(character);
    }

    return exo::call(character_stack, string_constructor, context);
}

exo::Object value_property(exo::Context const context) {
    auto const&& stack_library { exo::find_parent("stack", context) };
    auto const&& empty_stack { exo::get_property("empty", stack_library, context) };

    auto const&& argc { exo::get_argc() };
    auto const&& argv { exo::get_argv() };

    auto argument_stack { empty_stack };

    for (uint32_t argv_index { argc }; argv_index > 0; --argv_index) {
        auto const&& character_string { c_string_to_string(argv[argv_index - 1], context) };
        auto const&& push_function { exo::get_property("push", argument_stack, context) };
        argument_stack = exo::call(character_string, push_function, context);
    }

    return argument_stack;
}

extern "C" void create_native_object(exo::ConstructionContext const construction_context, exo::Context const context) {
    exo::define_property("value", &value_property, construction_context);
}
