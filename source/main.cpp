#include <core/application.h>

class Emulator : public IScene
{
public:
    Emulator();
    ~Emulator();

    void Init();
    void Update();
    void UpdateAfterPhysics();

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

    inline int Hex(const char *hexstring)
    {
        return (int)strtol(hexstring, NULL, 16);
    }

    int registerA;

    Array<Code> program;
    unsigned int programCounter;

    Array<int> memory;

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
            Log(memory[i]);
        }
    }
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

