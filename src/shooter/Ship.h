/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2017 (git@dga.me.uk)
 */
#pragma once

#include "ecs/Entity.h"
#include "ecs/Component.h"
#include "renderer/BillboardSet.h"
#include "renderer/Mesh.h"
#include "math/Defs.h"

using namespace dw;

class ShipCameraController : public Object {
public:
    DW_OBJECT(ShipCameraController);

    explicit ShipCameraController(Context* ctx, const Vec3& offset);
    ~ShipCameraController() = default;

    void follow(Entity* ship);
    void possess(Entity* camera);

    void update(float dt);

private:
    Entity* possessed_;
    Entity* followed_;

    Vec3 offset_;
};

class ShipEngineData {
public:
    ShipEngineData(const Vec3& force, const Vec3& offset);

    void fire(float power);
    void update(float dt);

    float activity() const;
    Vec3 force() const;
    Vec3 offset() const;

private:
    float activity_;
    Vec3 force_;
    Vec3 offset_;
};

class ShipEngineInstance {
public:
    ShipEngineInstance(ShipEngineData* parent, const Vec3& force, const Vec3& offset,
                       bool forwards);

    Vec3 force() const;
    Vec3 offset() const;
    Vec3 torque() const;
    ShipEngineData* parent() const;
    bool isForwards() const;

private:
    Vec3 force_;
    Vec3 offset_;
    ShipEngineData* parent_;
    bool forwards_;
};

class ShipEngines : public Component, public Object {
public:
    DW_OBJECT(ShipEngines);

    ShipEngines(Context* ctx, const Vector<ShipEngineData>& movement_engines,
                const Vector<ShipEngineData>& nav_engines);

    void onAddToEntity(Entity* parent) override;

    // Movement engines.
    void calculateMaxMovementForce(Vec3& pos_force, Vec3& neg_force);
    // power is proportional to max force.
    Vec3 fireMovementEngines(const Vec3& power);

    // Rotational engines.
    void calculateMaxRotationalTorque(Vec3& clockwise, Vec3& anticlockwise) const;
    // X - pitch, Y - yaw, Z - roll. power is proportional to max torque.
    Vec3 calculateRotationalTorque(const Vec3& power) const;
    Vec3 fireRotationalEngines(const Vec3& power);

    static Vec3 convertToPower(const Vec3& force, const Vec3& max_pos_force,
                               const Vec3& max_neg_force);

private:
    Vector<ShipEngineData> engine_data_;
    Vector<ShipEngineData> nav_engine_data_;
    SharedPtr<BillboardSet> glow_billboards_;
    SharedPtr<BillboardSet> trail_billboards_;

    // Navigational engines. [0] == x, [1] == y, [2] == z
    Array<Vector<ShipEngineInstance>, 3> movement_engines_;

    // Rotational engines.
    Array<Vector<ShipEngineInstance>, 3> navigation_engines_;

    friend class ShipEngineSystem;
};

class ShipEngineSystem : public System {
public:
    DW_OBJECT(ShipEngineSystem);

    explicit ShipEngineSystem(Context* ctx) : System(ctx) {
        supportsComponents<Transform, RigidBody, ShipEngines>();
    }

    void processEntity(Entity& entity, float dt) override {
        auto& transform = *entity.component<Transform>();
        auto& rigid_body = *entity.component<RigidBody>();
        auto& ship_engines = *entity.component<ShipEngines>();

        auto& engines = ship_engines.engine_data_;
        auto& nav_engines = ship_engines.nav_engine_data_;

        // Update particles.
        if (ship_engines.glow_billboards_) {
            for (int i = 0; i < engines.size(); i++) {
                int particle = i;
                float engine_glow_size = 4.0f * engines[i].activity();
                ship_engines.glow_billboards_->setParticlePosition(
                    particle, Vec3{transform.modelMatrix(Position::origin) *
                                   Vec4{engines[i].offset(), 1.0f}});
                ship_engines.glow_billboards_->setParticleSize(
                    particle, {engine_glow_size, engine_glow_size});
                ship_engines.trail_billboards_->setParticlePosition(
                    particle, Vec3{transform.modelMatrix(Position::origin) *
                                   Vec4{engines[i].offset(), 1.0f}});
                ship_engines.trail_billboards_->setParticleSize(
                    particle, {engine_glow_size * 0.5f, engine_glow_size * 6.0f});
                ship_engines.trail_billboards_->setParticleDirection(
                    particle, Vec3{transform.modelMatrix(Position::origin) *
                                   Vec4{-engines[i].force().Normalized(), 0.0f}});
            }
            for (int i = 0; i < nav_engines.size(); i++) {
                int particle = i + engines.size();
                float engine_glow_size = 2.0f * nav_engines[i].activity();
                ship_engines.glow_billboards_->setParticlePosition(
                    particle, Vec3{transform.modelMatrix(Position::origin) *
                                   Vec4{nav_engines[i].offset(), 1.0f}});
                ship_engines.glow_billboards_->setParticleSize(
                    particle, {engine_glow_size, engine_glow_size});
                ship_engines.trail_billboards_->setParticlePosition(
                    particle, Vec3{transform.modelMatrix(Position::origin) *
                                   Vec4{nav_engines[i].offset(), 1.0f}});
                ship_engines.trail_billboards_->setParticleSize(
                    particle, {engine_glow_size * 0.25f, engine_glow_size * 3.0f});
                ship_engines.trail_billboards_->setParticleDirection(
                    particle, Vec3{transform.modelMatrix(Position::origin) *
                                   Vec4{-nav_engines[i].force().Normalized(), 0.0f}});
            }
        }

        // Attenuate engines.
        for (auto& e : engines) {
            e.update(dt);
        }
        for (auto& e : nav_engines) {
            e.update(dt);
        }
    }
};

class Ship;

class ShipFlightComputer : public Object {
public:
    DW_OBJECT(ShipFlightComputer);

    ShipFlightComputer(Context* ctx, Ship* ship);

    void setTargetLinearVelocity(const Vec3& target_linear_velocity);
    void setTargetAngularVelocity(const Vec3& target_angular_velocity);

    void update(float dt);

private:
    Ship* ship_;
    Vec3 target_linear_velocity_;
    Vec3 target_angular_velocity_;

    Vec3 ship_acceleration_forwards_;
    Vec3 ship_acceleration_backwards_;
    Vec3 ship_angular_acceleration_forwards_;
    Vec3 ship_angular_acceleration_backwards_;
};

class Ship : public Object {
public:
    DW_OBJECT(Ship);

    Ship(Context* ctx);
    ~Ship() = default;

    void update(float dt);

    // Used by the flight computer.
    void fireMovementThrusters(const Vec3& power);
    void fireRotationalThrusters(const Vec3& power);
    Vec3 angularVelocity() const;
    Vec3 localVelocity() const;

    Entity* entity() const;

private:
    Entity* core_;
    btRigidBody* rb_;
    SharedPtr<Material> material_;

    SharedPtr<ShipFlightComputer> flight_computer_;
};
