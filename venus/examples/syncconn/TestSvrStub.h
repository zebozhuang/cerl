/*
	Description: 	Do not edit this file manually
	Author:			SDL Compiler
*/

#ifndef TESTSVR_TESTSVR_STUB_H
#define TESTSVR_TESTSVR_STUB_H

#ifndef TESTSVR_TESTSVR_IMPL_H
#include "TestSvrImpl.h"
#endif

namespace testsvr {

class TestSvrStub : public TestSvrImpl
{
public:
	template <class ArchiveT>
	void cerl_call _process(cerl::Caller* _caller, ArchiveT& _ar)
	{
		Caller* const caller = static_cast<Caller*>(_caller);
		cerl::FID fid = caller->getFid();
		if (is_async_call(fid))
		{
			_handle_cast(caller, fid, _ar);
		}
		else
		{
			_handle_call(caller, fid, _ar);
		}
	}

	template <class ArchiveT>
	void cerl_call _handle_cast(Caller* caller, cerl::FID _fid, ArchiveT& _ar)
	{
			skipMailBody(_ar);
			CERL_TRACE("cerl::handle_cast - Unknown FID: %.8X", _fid);
	}

	template <class ArchiveT>
	void cerl_call _handle_call(Caller* caller, cerl::FID _fid, ArchiveT& _ar)
	{
		switch (_fid)
		{
		case code_test:
			{
				_TestArgs _args;
				_TestResult _result = cerl::code_unknown_error;
				const bool _fOk = cerl::getMailBody(_ar, _args);

				if (_fOk)
				{
					CERL_DUMP_RECV_REQUEST(caller->getSelf(), this, _fid, _args);
					test(caller, _result, _args);
				}

				caller->dbg_reply(this, _result);
			}
			break;
		default:
			{
				cerl::handle_call(this, caller, _fid, _ar);
			}
			break;
		}
	}
};

inline void cerl_call TestSvrImpl::_run(cerl::Fiber self, SOCKET socketListen)
{
	cerl::ServerRunner<TestSvrStub>::run(self, static_cast<TestSvrStub*>(this), socketListen);
}

} //namespace

#endif /* TESTSVR_TESTSVR_STUB_H */ 
