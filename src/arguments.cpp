#include <native.hpp>

exo::Object character_to_binary(char const character, exo::Object const& self) {
    auto const&& bit_library { exo::find("bit", self) };
    auto const&& true_bit { exo::get("true", bit_library) };
    auto const&& false_bit { exo::get("false", bit_library) };

    auto const&& stack_library { exo::find("stack", self) };
    auto const&& empty_stack { exo::get("empty", stack_library) };

    auto const&& binary_library { exo::find("binary", self) };
    auto const&& binary_constructor { exo::get("of", binary_library) };

    auto bit_stack { empty_stack };

    for (uint8_t character_index { 8 }; character_index > 0; --character_index) {
        auto const&& index_is_one { character & (0b1 << (character_index - 1)) };
        auto const& bit_value { index_is_one ? true_bit : false_bit };

        auto const&& push_function { exo::get("push", bit_stack) };
        bit_stack = exo::call(bit_value, push_function);
    }

    return exo::call(bit_stack, binary_constructor);
}

exo::Object c_string_to_string(char const* character_address, exo::Object const& self) {
    auto const&& stack_library { exo::find("stack", self) };
    auto const&& empty_stack { exo::get("empty", stack_library) };

    auto const&& string_library { exo::find("string", self) };
    auto const&& string_constructor { exo::get("of", string_library) };

    auto character_stack { empty_stack };

    while (char const character = *character_address) {
        auto const&& binary_character { character_to_binary(character, self) };
        auto const&& push_function { exo::get("push", character_stack) };
        character_stack = exo::call(binary_character, push_function);

        character_address += sizeof(character);
    }

    return exo::call(character_stack, string_constructor);
}

exo::Object value_property(exo::Object const self) {
    auto const&& stack_library { exo::find("stack", self) };
    auto const&& empty_stack { exo::get("empty", stack_library) };

    auto const&& argc { exo::get_argc() };
    auto const&& argv { exo::get_argv() };

    auto argument_stack { empty_stack };

    for (uint32_t argv_index { 0 }; argv_index < argc; ++argv_index) {
        auto const&& character_string { c_string_to_string(argv[argv_index], self) };
        auto const&& push_function { exo::get("push", argument_stack) };
        argument_stack = exo::call(character_string, push_function);
    }

    return argument_stack;
}

extern "C" void create_native_object(exo::Context const context, exo::Object const self) {
    exo::define_property("value", &value_property, context);
}
