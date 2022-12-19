#pragma once

#include <slate/component/component.hpp>

class PunchComponent : public slate::Component  {

public:
	float force = 500.0f;
	float max_dist = 1.5f;
	virtual void update(std::shared_ptr<slate::Scene> scene, const float dt) override;
	
};