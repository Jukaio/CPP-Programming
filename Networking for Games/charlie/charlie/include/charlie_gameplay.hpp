// charlie_gameplay.hpp

#ifndef CHARLIE_GAMEPLAY_HPP_INCLUDED
#define CHARLIE_GAMEPLAY_HPP_INCLUDED

#include <charlie.hpp>
#include <map>
#include <typeinfo>
#include <type_traits>
#include <pugixml.hpp>
#include <string>

namespace charlie 
{
   namespace gameplay 
   {
       namespace collision
       {
           bool rect_rect_collision(Rectangle& p_lhs, Rectangle& p_rhs);
       }

	   enum class Action
	   {
		   UP = 1 << 0,
		   LEFT = 1 << 1,
		   DOWN = 1 << 2,
		   RIGHT = 1 << 3,
           BOMB = 1 << 4,

           COUNT
	   };



       struct Grid
       {
           Vector2 m_position;
           Vector2 m_tile_size;
           Vector2 m_world_dimensions;

           static void draw(const Grid& p_grid, Renderer& p_renderer, const Color& p_color, int p_scale);
           static void draw_position(const Grid& p_grid, Renderer& p_renderer, const Color& p_color, const Vector2& p_index, int p_scale);
           static Vector2 world_to_grid(const Grid& p_grid, const Vector2& p_world_pos);
           static Vector2 grid_to_world(const Grid& p_grid, const Vector2& p_grid_position);
       };

       struct Tilemap
       {
            enum TileType
            {
                NONE,
                BLOCK,
                DESTRUCTIBLE,
                WALKABLE
            };

            char m_tile_map[256];
            uint32 m_transfer_buffer[16] = { 0 };

            static void apply_received_data(Tilemap& p_map);
            static void prepare_sent_data(Tilemap& p_map);

            static void init(Tilemap& p_map, std::string p_path);
            static void set_data(Tilemap& p_map, const std::string& p_data);
            static void render(const Tilemap& p_map, Grid& p_grid, Renderer& p_renderer, const uint8 p_scale);
            static bool collides(Tilemap& p_map, Grid& p_grid, Vector2 p_position, Vector2 p_dimension);
            static char& get_tile_type(Tilemap& p_map, uint8 p_x, uint8 p_y);

       };

       struct Blast
       {
           bool m_active;
           Transform m_transform;
           Vector2 m_dimension;
       };

       struct Explosion
       {
           bool m_active;
           Blast m_blasts[32];

           uint32 m_tick_flag;
       };

       struct Bomb
       {
           bool m_active;
           Transform m_transform;
           Vector2 m_dimension;

           uint32 m_tick_flag;

           Explosion m_explosion;

           static void activate(Bomb& p_bomb, 
                                const Grid& p_grid, 
                                const Vector2& p_world_pos, 
                                const uint32& p_world_tick, 
                                const uint32& p_explode_in);
           static void update(Bomb& p_bomb, 
                              Tilemap& p_map, 
                              const Grid& p_grid, 
                              const uint32& p_world_tick, 
                              const uint32& p_explode_duration);
           static void clean_explosion(Explosion& p_explosion, 
                                       const uint32& p_world_ticks);
       };

       struct Player
       {
           bool m_active;
           Transform m_transform;
           Vector2 m_dimension;

           Bomb m_bomb;

           static void draw(Renderer& p_renderer, 
                            Player& p_player, 
                            int32 p_scale, 
                            Color p_color = Color::Red);
       };

	   typedef uint16 Entity;

	   template<typename T>
	   struct Component 
	   {
		   Component(T p_data)
			   : data(p_data)
		   {

		   }
		   T data;
	   };

	   struct EntityManger
	   {
		   const static int MAX_ENTITIES = 4000;

		   EntityManger()
		   {
			   m_signature.reserve(MAX_ENTITIES);
		   }

		   Entity create_entity()
		   {
			   auto to_return = m_available.back();
			   m_available.erase(m_available.begin());
			   m_signature.at(to_return) = 0;
			   return to_return;
		   }
		   void destroy_entity(Entity p_entity)
		   {
			   m_available.push_back(p_entity);
			   m_signature.at(p_entity) = 0;
		   }

		   void set_signature(Entity p_entity, uint32 p_signature)
		   {
			   m_signature.at(p_entity) = p_signature;

		   }

		   DynamicArray<uint32> m_signature;
		   DynamicArray<Entity> m_available;
	   };

	   struct ComponentManager
	   {
		   template<typename T>
		   void add_component(Entity p_entity, T component)
		   {
			   m_map[p_entity][typeid(T)] = Component<T>(component);
		   }

		   template<typename T>
		   T& get_component(Entity p_entity)
		   {
			   return m_map[p_entity][typeid(T)];
		   }
		   DynamicArray<std::map<std::type_info, Component<std::type_info>>> m_map;
	   };

	   struct System;

	   struct SystemManager
	   {
		   template<typename T>
		   void add_component(Entity p_entity, System* p_system)
		   {
			   m_map[typeid(T)] = p_system;
		   }

		   template<typename T>
		   T* get_component()
		   {
			   return m_map[typeid(T)];
		   }
		   std::map<std::type_info, System*> m_map;
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
