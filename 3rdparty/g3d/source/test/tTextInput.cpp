#include "../include/G3DAll.h"

void testTextInput() {
    printf("TextInput\n");

    {
        // Parse floats
 		TextInput ti(TextInput::FROM_STRING, ".1");
        debugAssert(ti.readNumber() == 0.1);
    }
    {
 		TextInput ti(TextInput::FROM_STRING, "..1");
        debugAssert(ti.readSymbol() == "..");
        debugAssert(ti.readNumber() == 1);
    }

    {
        // Quoted string with escapes.  The actual expression we are parsing looks like:
        // "\\"
 		TextInput ti(TextInput::FROM_STRING, "\"\\\\\"");
        Token t;

		ti.readString("\\");

        t = ti.read();
        debugAssert(t.type() == Token::END);
        debugAssert(!ti.hasMore());
	}

    {
        // Quoted string without escapes: read as two backslashes
        // (the test itself has to escape the backslashes, just to write them).
        // The actual expression we are parsing is:
        // "\"
        TextInput::Options opt;
        Token t;

        opt.escapeSequencesInStrings = false;
 		TextInput ti(TextInput::FROM_STRING, "\"\\\"", opt);
		ti.readString("\\");

        t = ti.read();
        debugAssert(t.type() == Token::END);
        debugAssert(!ti.hasMore());
	}

	{
 		TextInput ti(TextInput::FROM_STRING, "a \'foo\' bar");
        Token t;

		ti.readSymbol("a");

		t = ti.read();
		debugAssert(t.extendedType() == Token::SINGLE_QUOTED_TYPE);
		debugAssert(t.string() == "foo");
		ti.readSymbol("bar");

        t = ti.read();
        debugAssert(t.type() == Token::END);
        debugAssert(!ti.hasMore());
	}

    {
        TextInput ti(TextInput::FROM_STRING, "2.x");
        Token t;

        debugAssert(ti.readNumber() == 2);
        ti.readSymbol("x");

        t = ti.read();
        debugAssert(t.type() == Token::END);
        debugAssert(!ti.hasMore());
    }
    {
        TextInput ti(TextInput::FROM_STRING, "1.E7");
        Token t;

        debugAssert(ti.readNumber() == 1.E7);

        t = ti.read();
        debugAssert(t.type() == Token::END);
        debugAssert(!ti.hasMore());
    }

    {
        TextInput ti(TextInput::FROM_STRING, "\\123");
        Token t;
        t = ti.read();
        debugAssert(t.type() == Token::SYMBOL);
        debugAssert(t.string() == "\\");
        t = ti.read();
        debugAssert(t.type() == Token::NUMBER);
        debugAssert(t.number() == 123);

        t = ti.read();
        debugAssert(t.type() == Token::END);
        debugAssert(!ti.hasMore());
    }

    {
        TextInput::Options options;
        options.otherCommentCharacter = '#';

        TextInput ti(TextInput::FROM_STRING, "1#23\nA\\#2", options);
        Token t;
        t = ti.read();
        debugAssert(t.type() == Token::NUMBER);
        debugAssert(t.number() == 1);

        // skip the comment
        t = ti.read();
        debugAssert(t.type() == Token::SYMBOL);
        debugAssert(t.string() == "A");

        // Read escaped comment character
        t = ti.read();
        debugAssert(t.type() == Token::SYMBOL);
        debugAssert(t.string() == "#");

        t = ti.read();
        debugAssert(t.type() == Token::NUMBER);
        debugAssert(t.number() == 2);

        t = ti.read();
        debugAssert(t.type() == Token::END);
        debugAssert(!ti.hasMore());
    }

    {
        TextInput ti(TextInput::FROM_STRING, "0xFEED");

        Token t;
   
        t = ti.peek();
        debugAssert(t.type() == Token::NUMBER);
        double n = ti.readNumber();
        (void)n;
        debugAssert((int)n == 0xFEED);

        t = ti.read();
        debugAssert(t.type() == Token::END);
        debugAssert(!ti.hasMore());
    }

    {

    TextInput::Options opt;
    opt.cppComments = false;
    TextInput ti(TextInput::FROM_STRING, 
                 "if/*comment*/(x->y==-1e6){cout<<\"hello world\"}; // foo\nbar",
                 opt);

    Token a = ti.read();
    Token b = ti.read();
    Token c = ti.read();
    Token d = ti.read();
    Token e = ti.read();
    Token f = ti.read();
    double g = ti.readNumber();
    (void)g;
    Token h = ti.read();
    Token i = ti.read();
    Token j = ti.read();
    Token k = ti.read();
    Token L = ti.read();
    Token m = ti.read();
    Token n = ti.read();
    Token p = ti.read();
    Token q = ti.read();
    Token r = ti.read();
    Token s = ti.read();
    Token t = ti.read();

    debugAssert(a.type() == Token::SYMBOL);
    debugAssert(a.string() == "if");

    debugAssert(b.type() == Token::SYMBOL);
    debugAssert(b.string() == "(");

    debugAssert(c.type() == Token::SYMBOL);
    debugAssert(c.string() == "x");

    debugAssert(d.type() == Token::SYMBOL);
    debugAssert(d.string() == "->");

    debugAssert(e.type() == Token::SYMBOL);
    debugAssert(e.string() == "y");

    debugAssert(f.type() == Token::SYMBOL);
    debugAssert(f.string() == "==");

    debugAssert(g == -1e6);

    debugAssert(h.type() == Token::SYMBOL);
    debugAssert(h.string() == ")");

    debugAssert(i.type() == Token::SYMBOL);
    debugAssert(i.string() == "{");

    debugAssert(j.type() == Token::SYMBOL);
    debugAssert(j.string() == "cout");

    debugAssert(k.type() == Token::SYMBOL);
    debugAssert(k.string() == "<<");

    debugAssert(L.type() == Token::STRING);
    debugAssert(L.string() == "hello world");

    debugAssert(m.type() == Token::SYMBOL);
    debugAssert(m.string() == "}");

    debugAssert(n.type() == Token::SYMBOL);
    debugAssert(n.string() == ";");

    debugAssert(p.type() == Token::SYMBOL);
    debugAssert(p.string() == "/");

    debugAssert(q.type() == Token::SYMBOL);
    debugAssert(q.string() == "/");

    debugAssert(r.type() == Token::SYMBOL);
    debugAssert(r.string() == "foo");

    debugAssert(s.type() == Token::SYMBOL);
    debugAssert(s.string() == "bar");

    debugAssert(t.type() == Token::END);
    }
    
    {
        TextInput ti(TextInput::FROM_STRING, "-1 +1 2.6");

        Token t;
   
        t = ti.peek();
        debugAssert(t.type() == Token::NUMBER);
        double n = ti.readNumber();
        debugAssert(n == -1);

        t = ti.peek();
        debugAssert(t.type() == Token::NUMBER);
        n = ti.readNumber();
        debugAssert(n == 1);

        t = ti.peek();
        debugAssert(t.type() == Token::NUMBER);
        n = ti.readNumber();
        debugAssert(n == 2.6);
    }

    {
        TextInput ti(TextInput::FROM_STRING, "- 1 ---.51");

        Token t;
   
        t = ti.peek();
        debugAssert(t.type() == Token::SYMBOL);
        ti.readSymbol("-");

        t = ti.peek();
        debugAssert(t.type() == Token::NUMBER);
        double n = ti.readNumber();
        debugAssert(n == 1);

        t = ti.peek();
        debugAssert(t.type() == Token::SYMBOL);
        ti.readSymbol("--");

        t = ti.peek();
        debugAssert(t.type() == Token::NUMBER);
        n = ti.readNumber();
        debugAssert(n == -.51);
    }


    {
        G3D::TextInput::Options ti_opts;
        const std::string str = "'";
        ti_opts.singleQuotedStrings = false;

        G3D::TextInput ti(G3D::TextInput::FROM_STRING, str, ti_opts);

        G3D::Token t = ti.read();

        /*
        printf("t.string       = %s\n", t.string().c_str());
        printf("t.type         = %d\n", t.type());
        printf("t.extendedType = %d\n", t.extendedType());

        printf("\n");
        printf("SYMBOL         = %d\n", G3D::Token::SYMBOL);
        printf("END            = %d\n", G3D::Token::END);

        printf("\n");
        printf("SYMBOL_TYPE    = %d\n", G3D::Token::SYMBOL_TYPE);
        printf("END_TYPE       = %d\n", G3D::Token::END_TYPE);
        */

        alwaysAssertM(t.type() == G3D::Token::SYMBOL, "");
        alwaysAssertM(t.extendedType() == G3D::Token::SYMBOL_TYPE, "");
    }

#define CHECK_EXC_POS(e, lnum, chnum)                                        \
    do {                                                                     \
        alwaysAssertM((e).line == (lnum) && (e).character == (chnum), "");   \
    } while (0)
#define CHECK_TOKEN_POS(t, lnum, chnum)                                      \
    do {                                                                     \
        alwaysAssertM((t).line() == (lnum)                                   \
                      && (t).character() == (chnum), "");                    \
    } while (0)
#define CHECK_TOKEN_TYPE(t, typ, etyp)                                       \
    do {                                                                     \
        alwaysAssertM((t).type() == (typ), "");                              \
        alwaysAssertM((t).extendedType() == (etyp), "");                     \
    } while (0)
#define CHECK_SYM_TOKEN(ti, str, lnum, chnum)                                \
    do {                                                                     \
        Token _t;                                                            \
        _t = (ti).read();                                                    \
        CHECK_TOKEN_TYPE(_t, Token::SYMBOL, Token::SYMBOL_TYPE);             \
                                                                             \
        CHECK_TOKEN_POS(_t, (lnum), (chnum));                                \
        alwaysAssertM(_t.string() == (str), "");                             \
    } while (0)
#define CHECK_END_TOKEN(ti, lnum, chnum)                                     \
    do {                                                                     \
        Token _t;                                                            \
        _t = (ti).read();                                                    \
        CHECK_TOKEN_TYPE(_t, Token::END, Token::END_TYPE);                   \
        CHECK_TOKEN_POS(_t, (lnum), (chnum));                                \
    } while (0)

    // Basic line number checking test.  Formerly would skip over line
    // numbers (i.e., report 1, 3, 5, 7 as the lines for the tokens), because
    // the newline would be consumed, pushed back to the input stream, then
    // consumed again (reincrementing the line number.)
    {
        TextInput ti(TextInput::FROM_STRING, "foo\nbar\nbaz\n");
        CHECK_SYM_TOKEN(ti, "foo", 1, 1);
        CHECK_SYM_TOKEN(ti, "bar", 2, 1);
        CHECK_SYM_TOKEN(ti, "baz", 3, 1);
        CHECK_END_TOKEN(ti,        4, 1);
    }

#define CHECK_ONE_SPECIAL_SYM(s)                                             \
    do {                                                                     \
        TextInput ti(TextInput::FROM_STRING, "\n a" s "b\n ");               \
        CHECK_SYM_TOKEN(ti, "a", 2, 2);                                      \
        CHECK_SYM_TOKEN(ti,   s, 2, 3);                                      \
        CHECK_SYM_TOKEN(ti, "b", 2, 3 + strlen(s));                          \
        CHECK_END_TOKEN(ti,      3, 2);                                      \
    } while (0)

    CHECK_ONE_SPECIAL_SYM("@");
    CHECK_ONE_SPECIAL_SYM("(");
    CHECK_ONE_SPECIAL_SYM(")");
    CHECK_ONE_SPECIAL_SYM(",");
    CHECK_ONE_SPECIAL_SYM(";");
    CHECK_ONE_SPECIAL_SYM("{");
    CHECK_ONE_SPECIAL_SYM("}");
    CHECK_ONE_SPECIAL_SYM("[");
    CHECK_ONE_SPECIAL_SYM("]");
    CHECK_ONE_SPECIAL_SYM("#");
    CHECK_ONE_SPECIAL_SYM("$");
    CHECK_ONE_SPECIAL_SYM("?");

    CHECK_ONE_SPECIAL_SYM("-");
    CHECK_ONE_SPECIAL_SYM("--");
    CHECK_ONE_SPECIAL_SYM("-=");
    CHECK_ONE_SPECIAL_SYM("->");

    CHECK_ONE_SPECIAL_SYM("+");
    CHECK_ONE_SPECIAL_SYM("++");
    CHECK_ONE_SPECIAL_SYM("+=");

    CHECK_ONE_SPECIAL_SYM(":");
    CHECK_ONE_SPECIAL_SYM("::");

    CHECK_ONE_SPECIAL_SYM("*");
    CHECK_ONE_SPECIAL_SYM("*=");
    CHECK_ONE_SPECIAL_SYM("/");
    CHECK_ONE_SPECIAL_SYM("/=");
    CHECK_ONE_SPECIAL_SYM("!");
    CHECK_ONE_SPECIAL_SYM("!=");
    CHECK_ONE_SPECIAL_SYM("~");
    CHECK_ONE_SPECIAL_SYM("~=");
    CHECK_ONE_SPECIAL_SYM("=");
    CHECK_ONE_SPECIAL_SYM("==");
    CHECK_ONE_SPECIAL_SYM("^");
    // Formerly (mistakenly) tokenized as symbol "^"
    CHECK_ONE_SPECIAL_SYM("^=");

    CHECK_ONE_SPECIAL_SYM(">");
    CHECK_ONE_SPECIAL_SYM(">>");
    CHECK_ONE_SPECIAL_SYM(">=");
    CHECK_ONE_SPECIAL_SYM("<");
    CHECK_ONE_SPECIAL_SYM("<<");
    CHECK_ONE_SPECIAL_SYM("<=");
    CHECK_ONE_SPECIAL_SYM("|");
    CHECK_ONE_SPECIAL_SYM("||");
    CHECK_ONE_SPECIAL_SYM("|=");
    CHECK_ONE_SPECIAL_SYM("&");
    CHECK_ONE_SPECIAL_SYM("&&");
    CHECK_ONE_SPECIAL_SYM("&=");

    CHECK_ONE_SPECIAL_SYM("\\");

    CHECK_ONE_SPECIAL_SYM(".");
    CHECK_ONE_SPECIAL_SYM("..");
    CHECK_ONE_SPECIAL_SYM("...");

#undef CHECK_ONE_SPECIAL_SYM

    // Formerly would loop infinitely if EOF seen in multi-line comment.
    {
        TextInput ti(TextInput::FROM_STRING, "/* ... comment to end");
        CHECK_END_TOKEN(ti, 1, 22);
    }

    // Formerly would terminate quoted string after "foobar", having
    // mistaken \377 for EOF.
    {
        // This is a quoted string "foobarybaz", but with the 'y' replaced by
        // character 0xff (Latin-1 'y' with diaeresis a.k.a. HTML &yuml;).
        // It should parse into a quoted string with exactly those chars.

        TextInput ti(TextInput::FROM_STRING, "\"foobar\377baz\"");
        ti.readString("foobar\377baz");
        CHECK_END_TOKEN(ti, 1, 13);
    }

    {
        TextInput ti(TextInput::FROM_STRING, "[ foo \n  bar\n");
        bool got_exc;
        try {
            ti.readSymbols("[", "foo", "]");
        } catch (TextInput::WrongSymbol e) {
            got_exc = true;
            alwaysAssertM(e.expected == "]", "");
            alwaysAssertM(e.actual == "bar", "");
            CHECK_EXC_POS(e, 2, 3);
        }
        alwaysAssertM(got_exc, "");
    }

    // Test file pseudonym creation.
    {
        TextInput ti(TextInput::FROM_STRING, "foo");
        Token t;
        t = ti.read();
        CHECK_TOKEN_TYPE(t, Token::SYMBOL, Token::SYMBOL_TYPE);
        CHECK_TOKEN_POS(t, 1, 1);
        alwaysAssertM(t.string() == "foo", "");
    }
    
    // Test filename override.
    {
        TextInput::Options tio;
        tio.sourceFileName = "<stdin>";
        TextInput ti(TextInput::FROM_STRING, "foo", tio);
        Token t;
        t = ti.read();
        CHECK_TOKEN_TYPE(t, Token::SYMBOL, Token::SYMBOL_TYPE);
        CHECK_TOKEN_POS(t, 1, 1);
        alwaysAssertM(t.string() == "foo", "");
    }

    // Signed numbers, parsed two different ways
    {
        TextInput t(TextInput::FROM_STRING, "- 5");
        Token x = t.read();
        CHECK_TOKEN_TYPE(x, Token::SYMBOL, Token::SYMBOL_TYPE);
        alwaysAssertM(x.string() == "-", "");
        
        x = t.read();
        CHECK_TOKEN_TYPE(x, Token::NUMBER, Token::INTEGER_TYPE);
        alwaysAssertM(x.number() == 5, "");
    }

    {
        TextInput::Options opt;
        opt.signedNumbers = false;
        TextInput t(TextInput::FROM_STRING, "-5", opt);
        alwaysAssertM(t.readNumber() == -5, "");
    }

    {
        TextInput::Options opt;
        opt.signedNumbers = false;
        TextInput t(TextInput::FROM_STRING, "- 5", opt);
        try {
            t.readNumber();
            alwaysAssertM(false, "Incorrect parse");
        } catch (...) {
        }
    }

    // Test Nan and inf    
    {
        TextInput::Options opt;
        opt.msvcSpecials = true;
        TextInput t(TextInput::FROM_STRING, "-1.#INF00", opt);
        double n = t.readNumber();
        alwaysAssertM(n == -inf(), "");
    }
    {
        TextInput::Options opt;
        opt.msvcSpecials = true;
        TextInput t(TextInput::FROM_STRING, "1.#INF00", opt);
        alwaysAssertM(t.readNumber() == inf(), "");
    }
    {
        TextInput::Options opt;
        opt.msvcSpecials = true;
        TextInput t(TextInput::FROM_STRING, "-1.#IND00", opt);
        alwaysAssertM(isNaN(t.readNumber()), "");
    }
    {
        TextInput t(TextInput::FROM_STRING, "fafaosadoas");
        alwaysAssertM(t.hasMore(), "");
        t.readSymbol();
        alwaysAssertM(! t.hasMore(), "");
    }
    
}
