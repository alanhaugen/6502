#include <core/application.h>

class Emulator : public IScene
{
public:
    Emulator();
    ~Emulator();

    void Init();
    void Update();
    void UpdateAfterPhysics();
};

Emulator::Emulator()
{
}

Emulator::~Emulator()
{
}

void Emulator::Init()
{
}

void Emulator::Update()
{
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

