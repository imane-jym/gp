#ifndef _SERVER_EVENTDISPATCHER_H_
#define _SERVER_EVENTDISPATCHER_H_

#include <boost/signals2/signal.hpp>
#include <boost/function.hpp> 
#include <boost/unordered_map.hpp>

template<typename TEvent>
class EventDispatcher
{
	public:

		typedef boost::function<void (uint32, TEvent&)>								EventHandler;
		typedef std::list<EventHandler>												EventHandlerList;	
		typedef boost::unordered_map<uint32,  EventHandlerList>						EventHandlerMap;
		typedef boost::signals2::signal<void (uint32, TEvent&)>								EventSignal;


		EventDispatcher(void){}
		virtual ~EventDispatcher(void){}


		bool DispatchEvent(uint32 id, TEvent& e)
		{

			handler_EventFilter(id,e);

			typename EventHandlerMap::iterator it = m_handlers.find(id);

			if(it == m_handlers.end())
			{
				handler_UnhandledEvent(id, e);
				return false;
			}

			if(it->second.size() == 0)
			{
				handler_UnhandledEvent(id, e);
				return false;
			}

			typename EventHandlerList::iterator list_it = it->second.begin();

			for(; list_it != it->second.end(); ++list_it)
			{
				(*list_it)(id, e);
			}

			return true;
		}

		template<typename TObject>
			void AddEventHandler(TObject* pObj, uint32 id, const boost::function<void (TObject*, uint32 id, TEvent&)>& handler)
			{
				AddEventHandler(id, boost::bind(handler, pObj, _1, _2));
			}

		void AddEventHandler(uint32 id, const EventHandler& handler)
		{
			m_handlers[id].push_back(handler);
		}


		template<typename TObject>
			void AddUnhandledEventHandler(TObject* pObj, const boost::function<void (TObject*, uint32 id, TEvent&)>& handler)
			{
				AddUnhandledEventHandler(boost::bind(handler, pObj, _1, _2));
			}
		void AddUnhandledEventHandler(const EventHandler& handler)
		{
			handler_UnhandledEvent.connect(handler);
		}


		template<typename TObject>
			void AddEventFilter(TObject* pObj, const boost::function<void (TObject*, uint32 id, TEvent&)>& handler)
			{
				AddEventFilter(boost::bind(handler, pObj, _1, _2));
			}
		void AddEventFilter(const EventHandler& handler)
		{
			handler_EventFilter.connect(handler);
		}

		void Clear()
		{
			if(!handler_EventFilter.empty())
			{
				handler_EventFilter.disconnect_all_slots();
			}

			m_handlers.clear();

			if(!handler_UnhandledEvent.empty())
			{
				handler_UnhandledEvent.disconnect_all_slots();
			}
		}
	private:

	private:

		EventSignal												handler_EventFilter;

		EventHandlerMap											m_handlers;

		EventSignal												handler_UnhandledEvent;

};

#endif
