#include <iostream>

#include "MethodTraceAgent.h"
#include "jvmti.h"

using namespace std;

jvmtiEnv* MethodTraceAgent::m_jvmti = 0;
char* MethodTraceAgent::m_filter = 0;

MethodTraceAgent::~MethodTraceAgent() throw(AgentException)
{
    // 必须释放内存，防止内存泄露
    m_jvmti->Deallocate(reinterpret_cast<unsigned char*>(m_filter));
}

void MethodTraceAgent::Init(JavaVM *vm) const throw(AgentException){
    jvmtiEnv *jvmti = 0;
	jint ret = (vm)->GetEnv(reinterpret_cast<void**>(&jvmti), JVMTI_VERSION_1_0);
	if (ret != JNI_OK || jvmti == 0) {
		throw AgentException(JVMTI_ERROR_INTERNAL);
	}
	m_jvmti = jvmti;
}

void MethodTraceAgent::ParseOptions(const char* str) const throw(AgentException)
{
    if (str == 0)
        return;
	const size_t len = strlen(str);
	if (len == 0) 
		return;

  	// 必须做好内存复制工作
	jvmtiError error;
    error = m_jvmti->Allocate(len + 1,reinterpret_cast<unsigned char**>(&m_filter));
	CheckException(error);
    strcpy(m_filter, str);

    // 可以在这里进行参数解析的工作
	// ...
}

void MethodTraceAgent::AddCapability() const throw(AgentException)
{
    // 创建一个新的环境
    jvmtiCapabilities caps;
    memset(&caps, 0, sizeof(caps));
    caps.can_generate_method_entry_events = 1;
    
    // 设置当前环境
    jvmtiError error = m_jvmti->AddCapabilities(&caps);
	CheckException(error);
}
  
void MethodTraceAgent::RegisterEvent() const throw(AgentException)
{
    // 创建一个新的回调函数
    jvmtiEventCallbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.MethodEntry = &MethodTraceAgent::HandleMethodEntry;
    
    // 设置回调函数
    jvmtiError error;
    error = m_jvmti->SetEventCallbacks(&callbacks, static_cast<jint>(sizeof(callbacks)));
	CheckException(error);

	// 开启事件监听
	error = m_jvmti->SetEventNotificationMode(JVMTI_ENABLE, JVMTI_EVENT_METHOD_ENTRY, 0);
	CheckException(error);
}

void JNICALL MethodTraceAgent::HandleMethodEntry(jvmtiEnv* jvmti, JNIEnv* jni, jthread thread, jmethodID method)
{
	try {
        jvmtiError error;
        jclass clazz;
        char* name;
		char* signature;
        
		// 获得方法对应的类
        error = m_jvmti->GetMethodDeclaringClass(method, &clazz);
        CheckException(error);
        // 获得类的签名
        error = m_jvmti->GetClassSignature(clazz, &signature, 0);
        CheckException(error);
        // 获得方法名字
        error = m_jvmti->GetMethodName(method, &name, NULL, NULL);
        CheckException(error);
        
        // 根据参数过滤不必要的方法
		if(m_filter != 0){
			if (strcmp(m_filter, name) != 0)
				return;
		}			
		cout << signature<< " -> " << name << "(..)"<< endl;

        // 必须释放内存，避免内存泄露
        error = m_jvmti->Deallocate(reinterpret_cast<unsigned char*>(name));
		CheckException(error);
        error = m_jvmti->Deallocate(reinterpret_cast<unsigned char*>(signature));
		CheckException(error);

	} catch (AgentException& e) {
		cout << "Error when enter HandleMethodEntry: " << e.what() << " [" << e.ErrCode() << "]";
    }
}


