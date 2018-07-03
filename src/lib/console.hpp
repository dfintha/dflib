#if !defined(DFLIB_CONSOLE)
#define DFLIB_CONSOLE

#include <iostream>

namespace df {
    enum console_color {
        black           = 0,
        red             = 1,
        green           = 2,
        yellow          = 3,
        blue            = 4,
        magenta         = 5,
        cyan            = 6,
        gray            = 7,
        dark_gray       = black     + 60,
        light_red       = red       + 60,
        light_green     = green     + 60,
        light_yellow    = yellow    + 60,
        light_blue      = blue      + 60,
        light_magenta   = magenta   + 60,
        light_cyan      = cyan      + 60,
        white           = gray      + 60
    };

    enum console_effect {
        bold            = 1,
        italic          = 3,
        underlined      = 4,
        blink           = 5,
        reverse         = 7,
        strikethrough   = 9
    };

    class console_function {
    public:
        virtual ~console_function() noexcept { }
        virtual void execute(std::ostream& stream) const = 0;
    };

    class console_foreground final : public console_function {
    public:
        console_foreground(console_color c) : color(c) { }
        void execute(std::ostream& stream) const override;
    private:
        console_color color;
    };

    class console_background final : public console_function {
    public:
        console_background(console_color c) : color(c) { }
        void execute(std::ostream& stream) const override;
    private:
        console_color color;
    };


    class console_format final : public console_function {
    public:
        console_format(console_effect e) : effect(e) { }
        void execute(std::ostream& stream) const override;
    private:
        console_effect effect;
    };

    class console_reset final : public console_function {
    public:
        void execute(std::ostream& stream) const override;
    };

    std::ostream& operator<<(std::ostream& stream, const console_function& f);

    // ------------------------------------------------------------------ //

    void console_foreground::execute(std::ostream& stream) const {
        stream << "\x1B" << '[' << (static_cast<int>(color) + 30) << 'm';
    }

    void console_background::execute(std::ostream& stream) const {
        stream << "\x1B" << '[' << (static_cast<int>(color) + 40) << 'm';
    }

    void console_format::execute(std::ostream& stream) const {
        stream << "\x1B" << '[' << static_cast<int>(effect) << 'm';
    }

    void console_reset::execute(std::ostream& stream) const {
        stream << "\x1B" << '[' << 0 << 'm';
    }

    std::ostream& operator<<(std::ostream& stream, const console_function& f) {
        f.execute(stream);
        return stream;
    }
}

#endif
