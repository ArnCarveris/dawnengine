/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2017 (git@dga.me.uk)
 */
#include "Common.h"
#include "SceneManager.h"

namespace dw {

SceneManager::SceneManager(Context* context) : Object(context) {
}

SceneManager::~SceneManager() {
}

void SceneManager::update(float dt) {
}

Node* SceneManager::getRootNode() const {
    return root_node_.get();
}

void SceneManager::preRender(Camera_OLD* camera) {
}
}  // namespace dw
