#include <core/application.h>
#include <core/components/sprite.h>

class Emulator : public IScene
{
public:
    Emulator();
    ~Emulator();

    void Init();
    void Update();
    void UpdateAfterPhysics();

    inline int Hex(const char *hexstring)
    {
        return (int)strtol(hexstring, NULL, 16);
    }

private:
    enum INSTRUCTIONS
    {
        LDA, // LOAD ACCUMULATOR A
        STA  // STORE ACCUMULATOR A
    };

    struct Code
    {
        int instruction;
        int value;

        Code()
        {
        }

        Code(int instruction_, int value_)
        {
            instruction = instruction_;
            value = value_;
        }

        Code(String code)
        {
            Array<String> words = code.ToWords();

            String instructionWord = words[0];
            String valueWord = words[1];

            if (instructionWord == LDA)
            {
                instruction = LDA;
            }
            else if (instructionWord == STA)
            {
                instruction = STA;
            }
            else
            {
                LogWarning("Unkown instruction: " + instructionWord);
            }

            String prefix = valueWord[0];
            //int number = valueWord.SubString(1);
            int value_ = 0;

            if (prefix == "$") // hexadecimal
            {
                //value = Hex(value_);
            }
            else if (prefix == "#") // literal number
            {
                value = value_;
            }
            else // memory address
            {
                value = value_;
            }
        }
    };

    Sprite *pointer;

    int registerA;

    Array<Code> program;
    unsigned int programCounter;

    Array<int> memory;
    Array<Sprite*> screen;

    void Interpret(Code code);
    void Display();
};

Emulator::Emulator()
{
}

Emulator::~Emulator()
{
}

void Emulator::Interpret(Code code)
{
    switch (code.instruction)
    {
    case LDA:
        registerA = code.value;
        break;
    case STA:
        memory[code.value] = registerA;
        break;
    default:
        LogWarning("Unknown instruction: " + code.instruction);
    }
}

void Emulator::Init()
{
    programCounter = 0;
    memory.Resize(1024 * 10);

    program.Add(Code(LDA, Hex("01")));
    program.Add(Code(STA, Hex("0200")));
    program.Add(Code(LDA, Hex("05")));
    program.Add(Code(STA, Hex("0201")));
    program.Add(Code(LDA, Hex("08")));
    program.Add(Code(STA, Hex("0202")));

    pointer = new Sprite("data/cursor.png");
    components.Add(pointer);

    screen.Add(new Sprite(255,0,0,100,100));
    components.Add(screen[0]);

    // Clear memory
    for (int i = 0; i < memory.Size(); i++)
    {
        memory[i] = 0; 
    }
}

void Emulator::Update()
{
    if (programCounter < program.Size())
    {
        Interpret(program[programCounter++]);
    }

    Display();
}

void Emulator::Display()
{
    for (int i = Hex("0200"); i < Hex("05ff"); i++)
    {
        if (memory[i] != 0)
        {
            //Log(memory[i]);
        }
    }

    pointer->x = input.Mouse.x;
    pointer->y = input.Mouse.y;
}

void Emulator::UpdateAfterPhysics()
{
}

int main(int argc, char **argv)
{
    Application application(argc, argv);
    
    application.AddScene(new Emulator());
    
    return application.Exec();
}

