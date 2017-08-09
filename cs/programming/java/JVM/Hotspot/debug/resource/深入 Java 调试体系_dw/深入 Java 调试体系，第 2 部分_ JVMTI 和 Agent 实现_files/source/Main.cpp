#include <iostream>

#include "MethodTraceAgent.h"
#include "jvmti.h"

using namespace std;

JNIEXPORT jint JNICALL Agent_OnLoad(JavaVM *vm, char *options, void *reserved)
{
    cout << "Agent_OnLoad(" << vm << ")" << endl;
    try{
        
        MethodTraceAgent* agent = new MethodTraceAgent();
		agent->Init(vm);
        agent->ParseOptions(options);
        agent->AddCapability();
        agent->RegisterEvent();
        
    } catch (AgentException& e) {
        cout << "Error when enter HandleMethodEntry: " << e.what() << " [" << e.ErrCode() << "]";
		return JNI_ERR;
	}
    
	return JNI_OK;
}

JNIEXPORT void JNICALL Agent_OnUnload(JavaVM *vm)
{
    cout << "Agent_OnUnload(" << vm << ")" << endl;
}
