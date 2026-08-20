#include "MCPGame.h"

class FMCPGameModule : public IModuleInterface
{
public:
	virtual void StartupModule() override {}
	virtual void ShutdownModule() override {}
};

IMPLEMENT_MODULE(FMCPGameModule, MCPGame)
