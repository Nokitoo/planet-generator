namespace Window {

    struct Event {
        // TODO: add other events
        enum class Type : uint32_t {
            Close
        };

        Type type;
    };

}

