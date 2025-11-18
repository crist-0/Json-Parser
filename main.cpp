#include <iostream>
#include <fstream>
#include <string>
#include <vector>

using  std:: cout;


enum Tokentype{
        OBJECT,
        NONE,
        ARRAY,
        VALUE,
        STRING,
        NUMBER,
        WHITESPACE,
        LEFT_BRACE,
        RIGHT_BRACE,
        LEFT_PAREN,
        RIGHT_PAREN,
        COMA,
        COLLON,
        ARRAY_LEFT_BRACE,
        ARRAY_RIGHT_BRACE,
        SLASH,
        QUOTE,
        REVERSE_SOLIDUS,
        SOLIDUS,
        BACKSPACE,
        FORMFEED,
        LINEFEED,
        CR,
        HORIZONTAL_TAB,
        MINUS,
        TRUE,
        FALSE
    };
    

struct Token{
    int line;
    enum Tokentype type;
    std:: string text;
};

std:: vector <struct Token> tokenCollection;

bool isAtEnd(std:: string source, int current){
    return current >= source.length();
}
    
void report(std:: string errFormat){
    std:: cout << "Parsing Failed";
    std:: cerr << "Error in the Format " << errFormat;
}

char peek(std:: string source, int current)
{
    if (isAtEnd(source,current))
    {
        return '\0';
    }
    return source[current];
}

char peekNext(int current, std:: string source)
{
    if (current + 1 >= source.length())
    {
        return '\n';
    }
    return source[current+1];
    
}

char advance(int &current, std:: string source) {
    return source[current++];
}

void addToken(Tokentype s, int line,std:: string source) {
    cout << "Token : " << s << "\n";
    cout << "Source : " << source << "\n";
    Token t;
    t.type = s;
    t.line = line;
    t.text = source;
    tokenCollection.push_back(t);
}


void stringCase(std:: string source, int &current, int line, int start)
{
    while (peek(source, current) != '"' && !isAtEnd(source,current))
    {
        if (peek(source, current) == '\n')
        {
            line++;
        }
        advance(current,source);
    }

    if (isAtEnd(source,current))
    {
        cout << "Error : unterminated string.";
        return;
    }
    
    advance(current,source);
    

    std:: string value = source.substr(start, (current)-start);
    cout << "\nString value = "<< value << "\n";
    addToken(STRING,line,value);
}

Tokentype checkKeyword(int start, int length, const char* rest, Tokentype token,std:: string source)
{
    std:: string s = source.substr(start, length);
    cout << "false keyword";
    cout << source.substr(start, length) << "\n";
    if (source.substr(start, length) == rest)
    {
        cout << "success" << "\n";
        if (s == "false" || s == "FALSE")
        {
            return Tokentype::FALSE;
        }
        else if (s == "true" || s == "TRUE")
        {
            return Tokentype::TRUE;
        }
    }
    return Tokentype::NONE;
}

bool isDigit(char c)
{
    return c >= '0' && c <= '9';
}

std:: string number(int line, int start, int &current, std:: string source)
{
    static int i = 0;
    i++;

    while (isDigit(peek(source,current)))
    {
        cout << "hi";
        advance(current, source);
    }

    std:: string str = (source.substr(start, (current-1)-start+1));
    return str;
}

void scanToken(int start, int &current, std:: string source, int line)
{
    char c  = advance(current,source);
    switch (c)
    {
        
        case '{':
            {
                addToken(LEFT_BRACE,line,"{");
            }
            break;
        case '}':
            {
                addToken(RIGHT_BRACE,line,"}");
            }
            break;
        case '(':
            {
                addToken(LEFT_PAREN,line,"(");
            }
            break;
        case ')':
            {
                addToken(RIGHT_PAREN,line,")");
            }
            break;
        case ',':
            {
                addToken(COMA,line,",");
            }
            break;
        case '-':
            {
                addToken(MINUS,line,"-");
            }
            break;
        case ':':
            {
                addToken(COLLON,line,":");
            }
            break;

        case 't':
            {
                Tokentype t = checkKeyword(current,4,"true",TRUE, source);
                addToken(TRUE,line,source);
            }
            break;
        case 'f':
            {
                Tokentype t = checkKeyword(current,5,"false",FALSE, source);
                addToken(FALSE,line,source);
            }
            break;
            case '"':
            {

                stringCase(source, current, line, start);
            }
            break;

            case '\n':
            {
                line++;
            }
            break;
            case ' ':
            {
            }
            break;

            default:
            {

                if (isDigit(c))
                {
                    std::string str = number(line, start, current, source);
                    addToken(NUMBER,line,str);
                    break;
                }
                else
                {
                    std:: cerr << "Unexpected character.";
                }
            }
            break;
    }
    return;
}

class Scanner {
    public:

    
    void scanTokens(std:: string source)
    {
        int start = 0;
        int current = 0;
        int line = 1;
        
    while (!isAtEnd(source, current))
    {
        start = current;
        scanToken(start,current, source,line);
    }
    
}

};


int  main(int argc, char *argv[])
{
    std:: string sample;
    std:: fstream File;
    File.open("file_name.json", std::ios::in);
    
    std:: string str;
    std:: string file_contents;
    while (std::getline(File,str))
    {
        file_contents += str;
    }

    cout << file_contents << "\n";
    
    Scanner s;
    s.scanTokens(file_contents);   

    return 0;
}