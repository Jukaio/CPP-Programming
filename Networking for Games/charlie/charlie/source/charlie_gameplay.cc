// charlie_gameplay.cc

#include "charlie_gameplay.hpp"

namespace charlie {
   namespace gameplay {
      // static 
      uint32 ComponentBase::next()
      {
         static uint32 index_ = 0;
         return index_++;
      }

      ComponentContext::~ComponentContext()
      {
         for (auto &container : containers_) {
            delete container;
         }
      }

      // static 
      uint32 EventBase::next()
      {
         static uint32 index_ = 0;
         return index_++;
      }

      EventContext::~EventContext()
      {
         for (auto &queue : queues_) {
            delete queue;
         }
      }

      //static 
      uint32 SystemBase::next()
      {
         static uint32 index = 0;
         return index++;
      }

      System::System()
         : active_(false)
      {
      }

      bool System::is_active() const
      {
         return active_;
      }

      void System::activate()
      {
         active_ = true;
      }

      void System::deactivate()
      {
         active_ = false;
      }

      Stage::Stage()
         : active_(false)
      {
      }

      Stage::~Stage()
      {
         for (auto &system : systems_) {
            delete system;
         }
      }

      bool Stage::is_active() const
      {
         return active_;
      }

      void Stage::activate() 
      {
         active_ = true;
      }

      void Stage::deactivate() 
      {
         active_ = false;
      }

      void Stage::update(const Time &dt) 
      {
         for (auto &system : systems_) {
            if (!system->is_active()) {
               continue;
            }

            system->update(dt, components_, events_);
         }

         events_.clear_all();
      }

      void Stage::draw(Renderer &renderer)
      {
         for (auto &system : systems_) {
            if (!system->is_active()) {
               continue;
            }

            system->draw(renderer, components_, events_);
         }
      }
   } // !gameplay
} // !charlie
