#ifndef STINGRAYKIT_HOLDER_H
#define STINGRAYKIT_HOLDER_H


#include <stingraykit/function/bind.h>
#include <stingraykit/shared_ptr.h>


namespace stingray
{


	template < typename NativeType >
	class ScopedHolder
	{
		STINGRAYKIT_NONCOPYABLE(ScopedHolder);
		typedef function<void(NativeType&)>		CleanupFuncType;

	private:
		NativeType		_handle;
		CleanupFuncType	_cleanupFunc;
		bool			_valid;

	public:
		ScopedHolder(const CleanupFuncType& cleanupFunc)
			: _cleanupFunc(cleanupFunc), _valid(false)
		{ }

		ScopedHolder(const NativeType& handle, const CleanupFuncType& cleanupFunc)
			: _handle(handle), _cleanupFunc(cleanupFunc), _valid(true)
		{ }

		~ScopedHolder()					{ Cleanup(); }

		bool Valid() const				{ return _valid; }
		const NativeType& Get() const	{ Check();					return _handle; }
		NativeType Release()			{ Check(); _valid = false;	return _handle; }

		void Clear()
		{ Cleanup(); }

		void Set(const NativeType& handle)
		{
			Cleanup();
			_handle = handle;
			_valid = true;
		}

	private:
		void Check() const { STINGRAYKIT_CHECK(_valid, std::logic_error("ScopedHolder is not valid!")); }
		void Cleanup()
		{
			if (_valid)
				_cleanupFunc(_handle);
			_valid = false;
		}
	};


	template <>
	class ScopedHolder<void>
	{
		STINGRAYKIT_NONCOPYABLE(ScopedHolder);
		typedef function<void()>	CleanupFuncType;

	private:
		CleanupFuncType	_cleanupFunc;
		bool			_valid;

	public:
		ScopedHolder(const CleanupFuncType& cleanupFunc, bool valid = false)
			: _cleanupFunc(cleanupFunc), _valid(valid)
		{ }

		~ScopedHolder()			{ Cleanup(); }

		bool Valid() const		{ return _valid; }
		void Clear()			{ Cleanup(); _valid = false; }
		void Set()				{ Cleanup(); _valid = true; }

	private:
		void Check() const		{ STINGRAYKIT_CHECK(_valid, std::logic_error("ScopedHolder is not valid!")); }
		void Cleanup() const	{ if (_valid) _cleanupFunc(); }
	};


	template < typename NativeType >
	class SharedHolder
	{
		typedef ScopedHolder<NativeType> Impl;
		STINGRAYKIT_DECLARE_PTR(Impl);
		typedef function<void(NativeType)>		CleanupFuncType;

	private:
		ImplPtr	_impl;

	public:
		SharedHolder()
		{ }

		SharedHolder(const CleanupFuncType& cleanupFunc)
			: _impl(make_shared<Impl>(cleanupFunc))
		{ }

		SharedHolder(NativeType handle, const CleanupFuncType& cleanupFunc)
			: _impl(make_shared<Impl>(handle, cleanupFunc))
		{ }

		~SharedHolder()
		{ }

		bool Valid() const		{ return _impl && _impl->Valid(); }

		NativeType Get() const	{ return _impl->Get(); }

		void Clear()
		{ _impl->Clear(); }

		void Set(NativeType handle)
		{ _impl->Set(handle); }
	};


	template <>
	class SharedHolder<void>
	{
		typedef ScopedHolder<void> Impl;
		STINGRAYKIT_DECLARE_PTR(Impl);
		typedef function<void()>		CleanupFuncType;

	private:
		ImplPtr	_impl;

	public:
		SharedHolder()
		{ }

		SharedHolder(const CleanupFuncType& cleanupFunc)
			: _impl(make_shared<Impl>(cleanupFunc))
		{ }

		~SharedHolder()
		{ }

		bool Valid() const	{ return _impl && _impl->Valid(); }
		void Clear()		{ _impl->Clear(); }
		void Set()			{ _impl->Set(); }

		static SharedHolder<void> CreateInitialized(const CleanupFuncType& cleanupFunc)
		{
			SharedHolder<void> result(cleanupFunc);
			result.Set();
			return result;
		}
	};

}


#endif