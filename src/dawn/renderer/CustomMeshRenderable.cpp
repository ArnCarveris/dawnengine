/*
 * Dawn Engine
 * Written by David Avedissian (c) 2012-2017 (git@dga.me.uk)
 */
#include "Common.h"
#include "renderer/CustomMeshRenderable.h"
#include "renderer/Program.h"

namespace dw {
CustomMeshRenderable::CustomMeshRenderable(Context* context, SharedPtr<VertexBuffer> vertex_buffer,
                                           SharedPtr<IndexBuffer> index_buffer)
    : Object{context}, vertex_buffer_{vertex_buffer}, index_buffer_{index_buffer} {
}

CustomMeshRenderable::~CustomMeshRenderable() {
}

void CustomMeshRenderable::draw(Renderer* r, uint view, const Mat4& model_matrix) {
    u32 vertex_count = index_buffer_ ? index_buffer_->indexCount() : vertex_buffer_->vertexCount();
    r->setVertexBuffer(vertex_buffer_->internalHandle());
    if (index_buffer_) {
        r->setIndexBuffer(index_buffer_->internalHandle());
    }
    // TODO: Do this in the material class via a "bind" method.
    renderer->setUniform("model_matrix", model_matrix);
    material_->program()->prepareForRendering();
    r->submit(view, material_->program()->internalHandle(), vertex_count);
}

const VertexBuffer* CustomMeshRenderable::vertexBuffer() const {
    return vertex_buffer_.get();
}

const IndexBuffer* CustomMeshRenderable::indexBuffer() const {
    return index_buffer_.get();
}
}  // namespace dw
