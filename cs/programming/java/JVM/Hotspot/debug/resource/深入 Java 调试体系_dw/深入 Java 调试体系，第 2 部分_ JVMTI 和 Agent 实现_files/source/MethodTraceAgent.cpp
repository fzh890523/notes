#include <iostream>

#include "MethodTraceAgent.h"
#include "jvmti.h"

using namespace std;

jvmtiEnv* MethodTraceAgent::m_jvmti = 0;
char* MethodTraceAgent::m_filter = 0;

MethodTraceAgent::~MethodTraceAgent() throw(AgentException)
{
    // �����ͷ��ڴ棬��ֹ�ڴ�й¶
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

  	// ���������ڴ渴�ƹ���
	jvmtiError error;
    error = m_jvmti->Allocate(len + 1,reinterpret_cast<unsigned char**>(&m_filter));
	CheckException(error);
    strcpy(m_filter, str);

    // ������������в��������Ĺ���
	// ...
}

void MethodTraceAgent::AddCapability() const throw(AgentException)
{
    // ����һ���µĻ���
    jvmtiCapabilities caps;
    memset(&caps, 0, sizeof(caps));
    caps.can_generate_method_entry_events = 1;
    
    // ���õ�ǰ����
    jvmtiError error = m_jvmti->AddCapabilities(&caps);
	CheckException(error);
}
  
void MethodTraceAgent::RegisterEvent() const throw(AgentException)
{
    // ����һ���µĻص�����
    jvmtiEventCallbacks callbacks;
    memset(&callbacks, 0, sizeof(callbacks));
    callbacks.MethodEntry = &MethodTraceAgent::HandleMethodEntry;
    
    // ���ûص�����
    jvmtiError error;
    error = m_jvmti->SetEventCallbacks(&callbacks, static_cast<jint>(sizeof(callbacks)));
	CheckException(error);

	// �����¼�����
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
        
		// ��÷�����Ӧ����
        error = m_jvmti->GetMethodDeclaringClass(method, &clazz);
        CheckException(error);
        // ������ǩ��
        error = m_jvmti->GetClassSignature(clazz, &signature, 0);
        CheckException(error);
        // ��÷�������
        error = m_jvmti->GetMethodName(method, &name, NULL, NULL);
        CheckException(error);
        
        // ���ݲ������˲���Ҫ�ķ���
		if(m_filter != 0){
			if (strcmp(m_filter, name) != 0)
				return;
		}			
		cout << signature<< " -> " << name << "(..)"<< endl;

        // �����ͷ��ڴ棬�����ڴ�й¶
        error = m_jvmti->Deallocate(reinterpret_cast<unsigned char*>(name));
		CheckException(error);
        error = m_jvmti->Deallocate(reinterpret_cast<unsigned char*>(signature));
		CheckException(error);

	} catch (AgentException& e) {
		cout << "Error when enter HandleMethodEntry: " << e.what() << " [" << e.ErrCode() << "]";
    }
}


