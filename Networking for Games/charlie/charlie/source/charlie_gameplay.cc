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

      void Grid::draw(const Grid& p_grid, Renderer& p_renderer, const Color& p_color, int p_scale)
      {
      
          int pos_x = (int32)p_grid.m_position.x_;
          int pos_y = (int32)p_grid.m_position.y_;
          int tile_w = (int32)p_grid.m_tile_size.x_;
          int tile_h = (int32)p_grid.m_tile_size.y_;
          int w = int32(p_grid.m_world_dimensions.x_ / tile_w);
          int h = int32(p_grid.m_world_dimensions.y_ / tile_h);

          for(int y = 0; y < h; y++)
          {
              for(int x = 0; x < w; x++)
              {
                  Rectangle rect = { 0, 0, 0, 0 };
                  rect.x_ = pos_x + (x * (tile_w * p_scale));
                  rect.y_ = pos_y + (y * (tile_h * p_scale));
                  rect.width_ = tile_w * p_scale;
                  rect.height_ = tile_h * p_scale;
                  
                  p_renderer.render_rectangle(rect, p_color);
              }
          }
          
      }

      void Grid::draw_position(const Grid& p_grid, Renderer& p_renderer, const Color& p_color, const Vector2& p_index, int p_scale)
      {
          Rectangle rect = { 0, 0, 0, 0 };
          int tile_w = (int32)p_grid.m_tile_size.x_;
          int tile_h = (int32)p_grid.m_tile_size.y_;

          rect.x_ = int32(p_grid.m_position.x_
                          + (p_index.x_ * (tile_w * p_scale)));
          rect.y_ = int32(p_grid.m_position.y_
                          + (int32(p_index.y_) * (tile_h * p_scale)));
          rect.width_ = tile_w * p_scale;
          rect.height_ = tile_h * p_scale;

          p_renderer.render_rectangle_fill(rect, p_color);
      }
      Vector2 Grid::world_to_grid(const Grid& p_grid, const Vector2& p_world_pos)
      {
          float x = p_world_pos.x_;
          float y = p_world_pos.y_;

          x -= p_grid.m_position.x_;
          y -= p_grid.m_position.y_;

          x /= p_grid.m_tile_size.x_;
          y /= p_grid.m_tile_size.y_;

          return Vector2{ int32(x), int32(y) };
      }
      Vector2 Grid::grid_to_world(const Grid& p_grid, const Vector2& p_grid_position)
      {
          int32 x = int32(p_grid_position.x_);
          int32 y = int32(p_grid_position.y_);

          x *= int32(p_grid.m_tile_size.x_);
          y *= int32(p_grid.m_tile_size.y_);

          x += int32(p_grid.m_position.x_);
          y += int32(p_grid.m_position.y_);
          return Vector2{ x, y };
      }

      void Tilemap::apply_received_data(Tilemap& p_map)
      {
          const uint8 bit_count = 8;
          const uint8 bit_position_offset = 2;
          const uint8 bit_size = 0b11;

          for(int row = 0; row < 16; row++)
          {
              for(int bit_index = 0; bit_index < 16; bit_index++)
              {
                  int offset = (bit_position_offset * bit_index);   // Offset of the registers in bits 
                                                                    // This Tilemap uses 2 bits to display 
                                                                    // four different states 

                  char value = ((p_map.m_transfer_buffer[row] &     // Current row gets ANDed with the current index
                                    (bit_size << offset))           // shift the two bit entry by the index 
                                        >> offset);                 // shift it back to obtain the value
                  p_map.m_tile_map[(row * 16) + bit_index] = value;
              }
          }
      }

      void Tilemap::prepare_sent_data(Tilemap& p_map)
      {
          const uint8 bit_count = 8;
          const uint8 bit_position_offset = 2;
          const uint8 bit_size = 0b11;

          memset(p_map.m_transfer_buffer, 0, sizeof(p_map.m_transfer_buffer));

          // Designed to be 16 x 16 -> 256 
          for(int row = 0; row < 16; row++)
          {
              for(int bit_index = 0; bit_index < 16; bit_index++)
              {
                  uint32 value = (p_map.m_tile_map[(row * 16) + bit_index] & bit_size);
                  int offset = (bit_position_offset * bit_index);
                  p_map.m_transfer_buffer[row] |= (value << offset);
              }
          }
      }

      void Tilemap::init(Tilemap& p_map, std::string p_path)
      {

          pugi::xml_document document;
          pugi::xml_parse_result result = document.load_file(p_path.c_str());
          if(result.status != pugi::status_ok)
              return;

          auto map_string = document.child("map").child("layer").child("data").child_value();
          std::string test = map_string;

          auto c = test.c_str();
          std::string number = "";
          number.clear();

          int index = 0;
          while(*c != '\0')
          {
              if(*c == '\n' || *c == ',' || *c == ' ')
              {
                  if(number.size() != 0)
                  {
                      p_map.m_tile_map[index] = char(std::stoi(number));
                      index++;
                      number.clear();
                  }
              }
              else if(*c >= '0' && *c <= '9')
              {
                  number += *c;
              }
              c++;
          }
          p_map.m_tile_map[index] = '\0'; // Add zero termination
      }

      void Tilemap::set_data(Tilemap& p_map, const std::string& p_data)
      {
          strcpy_s(p_map.m_tile_map, p_data.c_str());
      }

      char& Tilemap::get_tile_type(Tilemap& p_map, uint8 p_x, uint8 p_y)
      {
          int index = (p_y * 16) + p_x;
          return p_map.m_tile_map[index];
      }

      void Tilemap::render(const Tilemap& p_map, Grid& p_grid, Renderer& p_renderer, const uint8 p_scale)
      {
          int pos_x = (int32)p_grid.m_position.x_;
          int pos_y = (int32)p_grid.m_position.y_;
          int tile_w = (int32)p_grid.m_tile_size.x_;
          int tile_h = (int32)p_grid.m_tile_size.y_;
          int w = int32(p_grid.m_world_dimensions.x_ / tile_w);
          int h = int32(p_grid.m_world_dimensions.y_ / tile_h);
          int index = -1;
          Rectangle rect = { 0, 0, 0, 0 };

          for(int y = 0; y < h; y++)
          {
              for(int x = 0; x < w; x++)
              {
                  index = (y * w) + x;

                  rect.x_ = pos_x + (x * (tile_w * p_scale));
                  rect.y_ = pos_y + (y * (tile_h * p_scale));
                  rect.width_ = tile_w * p_scale;
                  rect.height_ = tile_h * p_scale;

                  Color color = Color::Black;

                  switch(p_map.m_tile_map[index])
                  {
                      case 0b00:
                          color = Color::Black;
                          break;
                      case 0b01:
                          color = Color::Yellow;
                          break;
                      case 0b10:
                          color = Color::Red;
                          break;
                      case 0b11:
                          color = Color::Green;
                          break;
                  }


                  p_renderer.render_rectangle_fill(rect, color);
              }
          }
      }

      bool Tilemap::collides(Tilemap& p_map, Grid& p_grid, Vector2 p_position, Vector2 p_dimension)
      {
          Vector2 grid_index = Grid::world_to_grid(p_grid, 
                                                  { p_position.x_ + p_dimension.x_, 
                                                    p_position.y_ + p_dimension.y_ });

          Rectangle rect = { int32(p_position.x_),
                             int32(p_position.y_), 
                             int32(p_dimension.x_), 
                             int32(p_dimension.y_) };

          int pos_x = (int32)p_grid.m_position.x_;
          int pos_y = (int32)p_grid.m_position.y_;
          int tile_w = (int32)p_grid.m_tile_size.x_;
          int tile_h = (int32)p_grid.m_tile_size.y_;
          int w = int32(p_grid.m_world_dimensions.x_ / tile_w);
          int h = int32(p_grid.m_world_dimensions.y_ / tile_h);

          const uint32 start_y = (grid_index.y_ > 0) ? uint32(grid_index.y_ - 1) : 0;
          const uint32 start_x = (grid_index.x_ > 0) ? uint32(grid_index.x_ - 1) : 0;
          const uint32 end_y = (grid_index.y_ < h - 1) ? uint32(grid_index.y_ + 1) : h;
          const uint32 end_x = (grid_index.x_ < w - 1) ? uint32(grid_index.x_ + 1) : w;

          for(uint32 y = start_y; y < end_y; y++)
          {
              for(uint32 x = start_x; x < end_x; x++)
              {
                  if(p_map.m_tile_map[(y * w) + x] == 1 ||
                     p_map.m_tile_map[(y * w) + x] == 2)
                  {
                      Rectangle other = { 0, 0, 0, 0 };
                      other.x_ = pos_x + (x * (tile_w));
                      other.y_ = pos_y + (y * (tile_h));
                      other.width_ = tile_w;
                      other.height_ = tile_h;

                      if(collision::rect_rect_collision(rect, other))
                          return true;
                  }
              }
          }

          return false;
      }
      void Bomb::activate(Bomb& p_bomb, const Grid& p_grid, const Vector2& p_world_pos, const uint32& p_world_tick, const uint32& p_explode_in)
      {
          p_bomb.m_active = true;
          p_bomb.m_tick_flag = p_world_tick + p_explode_in;
          Vector2 grid_index = gameplay::Grid::world_to_grid(p_grid, p_world_pos + (p_bomb.m_dimension * 0.5f));

          Vector2 bomb_position = gameplay::Grid::grid_to_world(p_grid, grid_index);
          p_bomb.m_transform.set_position(bomb_position);
      }
      void Bomb::update(Bomb& p_bomb, Tilemap& p_map, const Grid& p_grid, const uint32& p_world_tick, const uint32& p_explode_duration)
      {
          if(p_bomb.m_active && p_world_tick >= p_bomb.m_tick_flag)
          {
              p_bomb.m_active = false;

              // Transition to the explosion

              auto& explosion = p_bomb.m_explosion;
              auto& blasts = explosion.m_blasts;

              explosion.m_active = true;
              explosion.m_tick_flag = p_world_tick + p_explode_duration;

              int width = int(p_grid.m_world_dimensions.x_ / p_grid.m_tile_size.x_);
              int height = int(p_grid.m_world_dimensions.y_ / p_grid.m_tile_size.y_);

              Vector2 temp = gameplay::Grid::world_to_grid(p_grid, p_bomb.m_transform.position_);
              const int x = int(temp.x_);
              const int y = int(temp.y_);
              const int blast_size = 3;
              const int half_size = blast_size / 2;
              const int start_x = (x - half_size > 0) ? x - blast_size - 1 : 0 - 1;
              const int start_y = (y - half_size > 0) ? y - blast_size - 1 : 0 - 1;
              const int end_x = (x + half_size < width - 1) ? x + blast_size + 1 : width;
              const int end_y = (y + half_size < height - 1) ? y + blast_size + 1 : height;

              int index = 0;
              auto blasting = [&p_map, &p_grid, &index](Blast& p_blast, uint8 p_x, uint8 p_y)
              {
                  auto& type = Tilemap::get_tile_type(p_map, uint8(p_x), uint8(p_y));
                  if(type == Tilemap::BLOCK)
                      return false;

                  p_blast.m_active = true;
                  p_blast.m_transform.set_position(gameplay::Grid::grid_to_world(p_grid, Vector2{ p_x, p_y }));
                  index++;

                  if(type == Tilemap::DESTRUCTIBLE)
                  {
                      type = Tilemap::WALKABLE;
                      return false;
                  }
                  return true;
              };

              for(int j = x - 1; j > start_x; j--)
              {
                  if(!blasting(blasts[index], uint8(j), uint8(y)))
                      break;
              }

              for(int j = x + 1; j < end_x; j++)
              {
                  if(!blasting(blasts[index], uint8(j), uint8(y)))
                      break;
              }

              for(int j = y - 1; j > start_y; j--)
              {
                  if(!blasting(blasts[index], uint8(x), uint8(j)))
                      break;
              }

              for(int j = y + 1; j < end_y; j++)
              {
                  if(!blasting(blasts[index], uint8(x), uint8(j)))
                      break;
              }

              blasts[index].m_active = true;
              blasts[index].m_transform.set_position(gameplay::Grid::grid_to_world(p_grid, Vector2{ x, y }));
          }
      }
      void Bomb::clean_explosion(Explosion& p_explosion, const uint32& p_world_ticks)
      {
          if(p_explosion.m_active && p_world_ticks >= p_explosion.m_tick_flag)
          {
              p_explosion.m_active = false;

              // Transition to end of explosion

              for(int k = 0; k < 32; k++)
              {
                  p_explosion.m_blasts[k].m_active = false;
              }
          }
      }
      void Player::draw(Renderer& p_renderer, Player& p_player, int32 p_scale, Color p_color)
      {
          if(p_player.m_active)
          {
              Rectangle rect = { int32(p_player.m_transform.position_.x_ * p_scale),
                  int32(p_player.m_transform.position_.y_ * p_scale),
                  int32(p_player.m_dimension.x_ * p_scale),
                  int32(p_player.m_dimension.y_ * p_scale) };
              p_renderer.render_rectangle_fill(rect, p_color);
          }
      }
} // !gameplay
} // !charlie

bool charlie::gameplay::collision::rect_rect_collision(Rectangle& p_lhs, Rectangle& p_rhs)
{
    return (p_lhs.x_ + p_lhs.width_ >= p_rhs.x_ &&
            p_lhs.x_ <= p_rhs.x_ + p_rhs.width_ &&
            p_lhs.y_ + p_lhs.height_ >= p_rhs.y_ &&
            p_lhs.y_ <= p_rhs.y_ + p_rhs.height_);
}
