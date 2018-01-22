#include "ShadowMap.hpp"

ShadowMap::ShadowMap(
	const std::shared_ptr<GraphicsDevice>& device,
	const std::shared_ptr<GraphicsContext>& context)
	: m_context(context) {

	initConstants(device);
}

ShadowMap::~ShadowMap() {
}

void ShadowMap::initConstants(const std::shared_ptr<GraphicsDevice>& device) {
	auto shadowMapStruct = ShadowMapStruct{ glm::mat4(1) };
	m_constantShadowMap = device->createConstantBuffer()
		.withLayout(shadowMapStructLayout)
		.withData(&shadowMapStruct)
		.build();
}

const static float OFFSET = 10;
const static glm::vec4 UP = glm::vec4(0, 1, 0, 0);
const static glm::vec4 FORWARD = glm::vec4(0, 0, -1, 0);
const static float SHADOW_DISTANCE = 100;

void ShadowMap::update(
	const std::shared_ptr<GameWindow>& window, 
	const std::shared_ptr<GameCamera>& camera, 
	const glm::vec3 sunPosition) {

	glm::vec3 lightDir = -glm::normalize(sunPosition);
	/*
	float lightPitch = acos(glm::length(glm::vec2(lightDir.x, lightDir.z)));
	float lightYaw = atan(lightDir.x / lightDir.z);
	lightYaw = (lightDir.z > 0 ? lightYaw - PI : lightYaw);
	
	glm::mat4 lightViewMatrix;
	lightViewMatrix = glm::rotate(glm::mat4(1), lightPitch, glm::vec3(1, 0, 0));
	lightViewMatrix = glm::rotate(lightViewMatrix, lightYaw, glm::vec3(0, 1, 0));

	float farHeight, farWidth, nearHeight, nearWidth;

	float fov = camera->getFOV() * PI / 180.0f;
	float aspect = float(window->getWidth()) / float(window->getHeight());
	farWidth = SHADOW_DISTANCE * tan(fov);
	nearWidth = camera->getZNear() * tan(fov);
	farHeight = farWidth / aspect;
	nearHeight = nearWidth / aspect;

	glm::mat4 rotation;
	rotation = glm::rotate(glm::mat4(1), camera->getRotation().x, glm::vec3(1, 0, 0));
	rotation = glm::rotate(glm::mat4(1), camera->getRotation().y, glm::vec3(0, 1, 0));
	rotation = glm::rotate(glm::mat4(1), camera->getRotation().z, glm::vec3(0, 0, 1));

	glm::vec3 forwardVector = rotation * FORWARD;

	glm::vec3 toFar = forwardVector * SHADOW_DISTANCE;
	glm::vec3 toNear = forwardVector * camera->getZNear();
	glm::vec3 centerNear = camera->getPosition() + toNear;
	glm::vec3 centerFar = camera->getPosition() + toFar;

	glm::vec3 upVector = rotation * UP;
	glm::vec3 rightVector = forwardVector * upVector;
	glm::vec3 downVector = -upVector;
	glm::vec3 leftVector = -rightVector;

	glm::vec3 farTop = centerFar + upVector * farHeight;
	glm::vec3 farBottom = centerFar + downVector * farHeight;
	glm::vec3 nearTop = centerNear + upVector * nearHeight;
	glm::vec3 nearBottom = centerNear + downVector * nearHeight;
	
	glm::vec4 points[8];
	points[0] = lightViewMatrix * glm::vec4(farTop + rightVector * farWidth, 1.0f);
	points[1] = lightViewMatrix * glm::vec4(farTop + leftVector * farWidth, 1.0f);
	points[2] = lightViewMatrix * glm::vec4(farBottom + rightVector * farWidth, 1.0f);
	points[3] = lightViewMatrix * glm::vec4(farBottom + leftVector * farWidth, 1.0f);
	points[4] = lightViewMatrix * glm::vec4(nearTop + rightVector * nearWidth, 1.0f);
	points[5] = lightViewMatrix * glm::vec4(nearTop + leftVector * nearWidth, 1.0f);
	points[6] = lightViewMatrix * glm::vec4(nearBottom + rightVector * nearWidth, 1.0f);
	points[7] = lightViewMatrix * glm::vec4(nearBottom + leftVector * nearWidth, 1.0f);

	float minX = FLT_MAX, maxX = FLT_MIN;
	float minY = FLT_MAX, maxY = FLT_MIN;
	float minZ = FLT_MAX, maxZ = FLT_MIN;
	for (unsigned i = 0; i < 8; ++i) {
		if (points[i].x > maxX) maxX = points[i].x;
		else if (points[i].x < minX) minX = points[i].x;
		else if (points[i].y > maxY) maxY = points[i].y;
		else if (points[i].y < minY) minY = points[i].y;
		else if (points[i].z > maxZ) maxZ = points[i].z;
		else if (points[i].z < minZ) minZ = points[i].z;
	}
	maxZ += OFFSET;

	glm::vec3 center = glm::vec3((minX + maxX) / 2.0f, (minY + maxY) / 2.0f, (minZ + maxZ) / 2.0f);
	center = glm::inverse(lightViewMatrix) * glm::vec4(center, 1.0f);

	ShadowMapStruct shadowMapStruct;
	shadowMapStruct.shadowViewProj = glm::mat4(
		2.0f / (maxX - minX), 0, 0, 0,
		0, 2.0f / (maxY - minY), 0, 0,
		0, 0, -2.0f / (maxZ - minZ), 0,
		0, 0, 0, 1);
	shadowMapStruct.shadowViewProj = glm::translate(shadowMapStruct.shadowViewProj, center);
	*/

	ShadowMapStruct shadowMapStruct;
	glm::mat4 shadowProj = glm::ortho<float>(-200, 200, -200, 200, camera->getZNear(), camera->getZFar());
	glm::mat4 shadowView = glm::lookAt(camera->getPosition() + lightDir, glm::vec3(0), glm::vec3(0, 1, 0));
	shadowMapStruct.shadowViewProj = shadowProj * shadowView;

	m_context->setData(m_constantShadowMap, &shadowMapStruct);
}