#ifndef LUA_BINDING_HPP
#define LUA_BINDING_HPP

#include "lib/ecs.hpp"
#include "systems/movement.hpp"

#include <sol/sol.hpp>

#include <memory>


namespace lua_binding {
	namespace context {
		std::shared_ptr<Scene> scene;
		sol::state lua;
		
		auto print_on_error = [](lua_State*, sol::protected_function_result result) {
			if (!result.valid()) {
				sol::error err = result;
				std::cerr << "Lua script failed to run.\n"
						  << err.what()
						  << std::endl;
			}
			return result;
		};

		Vec3 getPosition(Entity entity) {
			if (scene == nullptr) {
				return {0, 0, 0};
			}
			if (!scene->hasComponent<TransformComponent>(entity)) {
				return {0, 0, 0};
			}
			return scene->getComponent<TransformComponent>(entity)->position;
		}

		void setPosition(Entity entity, Vec3 position) {
			if (scene == nullptr) {
				return;
			}
			if (!scene->hasComponent<TransformComponent>(entity)) {
				return;
			}
			
			Vec3& pos = scene->getComponent<TransformComponent>(entity)->position;
				
			pos.x = position.x;
			pos.y = position.y;
			pos.z = position.z;
		}
		
		void initTypes() {
			lua.new_usertype<Vec3>("Vec3", 
				"x",
				&Vec3::x,
				"y",
				&Vec3::y,
				"z",
				&Vec3::z
			);
		}
		
		void initFunctions() {
			lua.set_function("get_position", 
				[]() { 
					return getPosition(lua.get<Entity>("entity")); 
				}
			);
			lua.set_function("set_position", 
				[](float x, float y, float z) { 
					return setPosition(lua.get<Entity>("entity"), {x, y, z}); 
				}
			);
		}
	};
	
	
	void setScene(Scene& scene) {
		context::scene = std::make_shared<Scene>(scene);
	}
	
	void init(Scene& scene) {
		setScene(scene);
		
		context::lua.open_libraries(sol::lib::base);
		
		context::initTypes();
		context::initFunctions();
	}
	
	
	void entityUpdate(Entity entity) {
		const char* path = "scripts/entityUpdate.lua";
		
		context::lua["entity"] = entity;
		context::lua.safe_script_file(path, context::print_on_error);
	}
};

#endif