
#include <emscripten.h>

extern "C" {

// Not using size_t for array indices as the values used by the javascript code are signed.
void array_bounds_check(const int array_size, const int array_idx) {
  if (array_idx < 0 || array_idx >= array_size) {
    EM_ASM_INT({
      throw 'Array index ' + $0 + ' out of bounds: [0,' + $1 + ')';
    }, array_idx, array_size);
  }
}

// conchParticleTemplate2D

conchParticleTemplate2D* EMSCRIPTEN_KEEPALIVE emscripten_bind_conchParticleTemplate2D_conchParticleTemplate2D_0() {
  return new conchParticleTemplate2D();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_conchParticleTemplate2D_setShader_3(conchParticleTemplate2D* self, int arg0, char* arg1, char* arg2) {
  self->setShader(arg0, arg1, arg2);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_conchParticleTemplate2D_getID_0(conchParticleTemplate2D* self) {
  return self->getID();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_conchParticleTemplate2D___destroy___0(conchParticleTemplate2D* self) {
  delete self;
}

// conchGraphics

conchGraphics* EMSCRIPTEN_KEEPALIVE emscripten_bind_conchGraphics_conchGraphics_0() {
  return new conchGraphics();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_conchGraphics_getID_0(conchGraphics* self) {
  return self->getID();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_conchGraphics___destroy___0(conchGraphics* self) {
  delete self;
}

// conchNode2D

conchNode2D* EMSCRIPTEN_KEEPALIVE emscripten_bind_conchNode2D_conchNode2D_0() {
  return new conchNode2D();
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_conchNode2D_getID_0(conchNode2D* self) {
  return self->getID();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_conchNode2D_setRootNode_0(conchNode2D* self) {
  self->setRootNode();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_conchNode2D___destroy___0(conchNode2D* self) {
  delete self;
}

// context

context* EMSCRIPTEN_KEEPALIVE emscripten_bind_context_context_0() {
  return new context();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_context_setSize_2(context* self, int arg0, int arg1) {
  self->setSize(arg0, arg1);
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_context_setType_1(context* self, int arg0) {
  self->setType(arg0);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_context_getID_0(context* self) {
  return self->getID();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_context___destroy___0(context* self) {
  delete self;
}

// VoidPtr

void EMSCRIPTEN_KEEPALIVE emscripten_bind_VoidPtr___destroy___0(void** self) {
  delete self;
}

// conchImage

conchImage* EMSCRIPTEN_KEEPALIVE emscripten_bind_conchImage_conchImage_0() {
  return new conchImage();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_conchImage_setImageInfo_3(conchImage* self, char* arg0, int arg1, int arg2) {
  self->setImageInfo(arg0, arg1, arg2);
}

int EMSCRIPTEN_KEEPALIVE emscripten_bind_conchImage_getImageID_0(conchImage* self) {
  return self->getImageID();
}

void EMSCRIPTEN_KEEPALIVE emscripten_bind_conchImage___destroy___0(conchImage* self) {
  delete self;
}

}

