// charlie_gameplay.hpp

#ifndef CHARLIE_GAMEPLAY_HPP_INCLUDED
#define CHARLIE_GAMEPLAY_HPP_INCLUDED

#include <charlie.hpp>

namespace charlie {
   namespace gameplay {
      struct Entity {
         Vector2 position_;
      };

      struct ComponentBase {
         static uint32 next();
      };

      template <typename T>
      struct ComponentType : ComponentBase {
         static uint32 type() 
         {
            static const uint32 type_ = ComponentBase::next();
            return type_;
         }
      };

      template <typename T>
      struct ComponentHandle {
         static constexpr uint32 INVALID = ~0u;

         ComponentHandle() 
            : index_(INVALID)
         {
         }

         explicit ComponentHandle(const uint32 index)
            : index_(index)
         {
         }

         bool is_valid() const
         {
            return index_ != INVALID;
         }

         uint32 index_;
      };

      struct ComponentContainerBase {
         virtual ~ComponentContainerBase() = default;
      };

      template <typename T>
      struct ComponentContainer final : ComponentContainerBase {
         template <typename ...Ts>
         ComponentHandle<T> create_component(Ts&& ...ts)
         {
            const uint32 index = static_cast<uint32>(components_.size());
            components_.emplace_back(std::forward<Ts>(ts)...);
            return ComponentHandle<T>(index);
         }
         
         T &get_component(const ComponentHandle<T> &handle)
         {
            assert(handle.is_valid());
            assert(components_.size() < handle.index_);
            return components_[handle.index_];
         }

         const T &get_component(const ComponentHandle<T> &handle) const
         {
            assert(handle.is_valid());
            assert(components_.size() < handle.index_);
            return components_[handle.index_];
         }

      private:
         DynamicArray<T> components_;
      };

      struct ComponentContext {
         ~ComponentContext();

         template <typename T, typename ...Ts>
         ComponentHandle<T> create_component(Ts ...ts)
         {
            return get_container<T>().create_component(std::forward<Ts>(ts)...);
         }

         template <typename T>
         T &get_component(const ComponentHandle<T> &handle)
         {
            return get_container<T>().get_component(handle);
         }

         template <typename T>
         const T &get_component(const ComponentHandle<T> &handle) const
         {
            return get_container<T>().get_component(handle);
         }

      private:
         template <typename T>
         ComponentContainer<T> &get_container()
         {
            const uint32 index = ComponentType<T>::type();
            if (containers_.size() < (index + 1)) {
               containers_.resize(index + 1);
               containers_[index] = new ComponentContainer<T>;
            }

            assert(containers_[index]);
            return *static_cast<ComponentContainer<T> *>(containers_[index]);
         }

         DynamicArray<ComponentContainerBase *> containers_;
      };

      struct EventBase {
         static uint32 next();
      };

      template <typename T>
      struct EventType : EventBase {
         static uint32 type()
         {
            static const uint32 index_ = EventBase::next();
            return index_;
         }
      };

      struct EventQueueBase {
         virtual ~EventQueueBase() = default;
         virtual void clear() = 0;
      };

      template <typename T>
      struct EventQueue final : EventQueueBase {
         auto begin()
         {
            return events_.begin();
         }

         const auto begin() const
         {
            return events_.begin();
         }

         auto end()
         {
            return events_.end();
         }

         const auto end() const
         {
            return events_.end();
         }

         virtual void clear()
         {
            events_.clear();
         }

         template <typename ...Ts>
         void push(Ts ...ts)
         {
            events_.emplace_back(std::forward<Ts>(ts)...);
         }

      private:
         DynamicArray<T> events_;
      };

      struct EventContext {
         ~EventContext();

         template <typename T, typename ...Ts>
         void push(Ts ...ts)
         {
            get<T>().push(std::forward<Ts>(ts)...);
         }

         template <typename T, typename Fn>
         void each(Fn &&fn)
         {
            for (auto &event : get<T>()) {
               fn(event);
            }
         }

         template <typename T>
         void clear()
         {
            get<T>().clear();
         }

         void clear_all()
         {
            for (auto &queue : queues_) {
               if (queue) {
                  queue->clear();
               }
            }
         }

      private:
         template <typename T>
         inline uint32 ensure_queue()
         {
            const uint32 index = EventType<T>::type();
            if (queues_.size() < (index + 1)) {
               queues_.resize(index + 1);
               queues_[index] = new EventQueue<T>;
            }
            return index;
         }

         template <typename T>
         EventQueue<T> &get()
         {
            const uint32 index = ensure_queue<T>();
            return *static_cast<EventQueue<T> *>(queues_[index]);
         }
         
         DynamicArray<EventQueueBase *> queues_;
      };

      struct SystemBase {
         static uint32 next();
      };

      template <typename T>
      struct SystemType : SystemBase {
         static uint32 type()
         {
            static const uint32 type_ = SystemBase::next();
            return type_;
         }
      };

      struct System {
         System();
         virtual ~System() = default;

         bool is_active() const;
         void activate();
         void deactivate();

         virtual bool init(ComponentContext &components) = 0;
         virtual void update(const Time &dt, ComponentContext &components, EventContext &events) = 0;
         virtual void draw(Renderer &renderer, ComponentContext &componens, EventContext &events) = 0;

      protected:
         bool active_;
      };

      struct Stage {
         Stage();
         ~Stage();

         bool is_active() const;
         void activate();
         void deactivate();

         void update(const Time &dt);
         void draw(Renderer &renderer);

         template <typename T, typename ...Ts>
         void add_system(Ts ...ts)
         {
            const uint32 index = SystemType<T>::type();
            if (systems_.size() < (index + 1)) {
               systems_.resize(index + 1);
               systems_[index] = nullptr;
            }

            assert(systems_[index] == nullptr);
            systems_[index] = new T(std::forward<Ts>(ts)...);
            assert(systems_[index] != nullptr);
            systems_[index]->init(components_);
         }

      private:
         bool active_;
         EventContext events_;
         ComponentContext components_;
         DynamicArray<System *> systems_;
      };
   } // !gameplay
} // !charlie

#endif // !CHARLIE_GAMEPLAY_HPP_INCLUDED
