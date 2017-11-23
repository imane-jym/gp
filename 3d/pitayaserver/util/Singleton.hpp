#ifndef __SINGLETON_H___
#define __SINGLETON_H___

template<typename T>
class Singleton
{
	public:
		static T* getInstance ()
		{
		//	mutex_.lock();
			if (instance_ == 0)
			{
				instance_ = new T();
			}
		//	mutex_.unlock();

			return instance_;
		}

		static T& GetSystem()
		{
			return *getInstance();
		}

		virtual ~Singleton ()
		{
			instance_ = 0;
		}
	private:
		static T* instance_;
		//static QMutex mutex_;
	protected:
		Singleton () {;}
};
template<typename T> T* Singleton<T>::instance_ = NULL;
//template<typename T> QMutex Singleton<T>::mutex_;

#endif /**/
