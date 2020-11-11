
// Bindings utilities

function WrapperObject() {
}
WrapperObject.prototype = Object.create(WrapperObject.prototype);
WrapperObject.prototype.constructor = WrapperObject;
WrapperObject.prototype.__class__ = WrapperObject;
WrapperObject.__cache__ = {};
Module['WrapperObject'] = WrapperObject;

function getCache(__class__) {
  return (__class__ || WrapperObject).__cache__;
}
Module['getCache'] = getCache;

function wrapPointer(ptr, __class__) {
  var cache = getCache(__class__);
  var ret = cache[ptr];
  if (ret) return ret;
  ret = Object.create((__class__ || WrapperObject).prototype);
  ret.ptr = ptr;
  return cache[ptr] = ret;
}
Module['wrapPointer'] = wrapPointer;

function castObject(obj, __class__) {
  return wrapPointer(obj.ptr, __class__);
}
Module['castObject'] = castObject;

Module['NULL'] = wrapPointer(0);

function destroy(obj) {
  if (!obj['__destroy__']) throw 'Error: Cannot destroy object. (Did you create it yourself?)';
  obj['__destroy__']();
  // Remove from cache, so the object can be GC'd and refs added onto it released
  delete getCache(obj.__class__)[obj.ptr];
}
Module['destroy'] = destroy;

function compare(obj1, obj2) {
  return obj1.ptr === obj2.ptr;
}
Module['compare'] = compare;

function getPointer(obj) {
  return obj.ptr;
}
Module['getPointer'] = getPointer;

function getClass(obj) {
  return obj.__class__;
}
Module['getClass'] = getClass;

// Converts big (string or array) values into a C-style storage, in temporary space

var ensureCache = {
  buffer: 0,  // the main buffer of temporary storage
  size: 0,   // the size of buffer
  pos: 0,    // the next free offset in buffer
  temps: [], // extra allocations
  needed: 0, // the total size we need next time

  prepare: function() {
    if (ensureCache.needed) {
      // clear the temps
      for (var i = 0; i < ensureCache.temps.length; i++) {
        Module['_free'](ensureCache.temps[i]);
      }
      ensureCache.temps.length = 0;
      // prepare to allocate a bigger buffer
      Module['_free'](ensureCache.buffer);
      ensureCache.buffer = 0;
      ensureCache.size += ensureCache.needed;
      // clean up
      ensureCache.needed = 0;
    }
    if (!ensureCache.buffer) { // happens first time, or when we need to grow
      ensureCache.size += 128; // heuristic, avoid many small grow events
      ensureCache.buffer = Module['_malloc'](ensureCache.size);
      assert(ensureCache.buffer);
    }
    ensureCache.pos = 0;
  },
  alloc: function(array, view) {
    assert(ensureCache.buffer);
    var bytes = view.BYTES_PER_ELEMENT;
    var len = array.length * bytes;
    len = (len + 7) & -8; // keep things aligned to 8 byte boundaries
    var ret;
    if (ensureCache.pos + len >= ensureCache.size) {
      // we failed to allocate in the buffer, ensureCache time around :(
      assert(len > 0); // null terminator, at least
      ensureCache.needed += len;
      ret = Module['_malloc'](len);
      ensureCache.temps.push(ret);
    } else {
      // we can allocate in the buffer
      ret = ensureCache.buffer + ensureCache.pos;
      ensureCache.pos += len;
    }
    return ret;
  },  
  copy: function(array, view, offset) {
    var offsetShifted = offset;
    var bytes = view.BYTES_PER_ELEMENT;
    switch (bytes) {
      case 2: offsetShifted >>= 1; break;
      case 4: offsetShifted >>= 2; break;
      case 8: offsetShifted >>= 3; break;
    }
    for (var i = 0; i < array.length; i++) {
      view[offsetShifted + i] = array[i];
    }   
  },
};

function ensureString(value) {
  if (typeof value === 'string') {
    var intArray = intArrayFromString(value);
    var offset = ensureCache.alloc(intArray, HEAP8);
    ensureCache.copy(intArray, HEAP8, offset);
    return offset;
  }
  return value;
}
function ensureInt8(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP8);
    ensureCache.copy(value, HEAP8, offset);
    return offset;
  }
  return value;
}
function ensureInt16(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP16);
    ensureCache.copy(value, HEAP16, offset);
    return offset;
  }
  return value;
}
function ensureInt32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAP32);
    ensureCache.copy(value, HEAP32, offset);
    return offset;
  }
  return value;
}
function ensureFloat32(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF32);
    ensureCache.copy(value, HEAPF32, offset);
    return offset;
  }
  return value;
}
function ensureFloat64(value) {
  if (typeof value === 'object') {
    var offset = ensureCache.alloc(value, HEAPF64);
    ensureCache.copy(value, HEAPF64, offset);
    return offset;
  }
  return value;
}


// _conchParticleTemplate2D
function _conchParticleTemplate2D() {
  this.ptr = _emscripten_bind_conchParticleTemplate2D_conchParticleTemplate2D_0();
  getCache(_conchParticleTemplate2D)[this.ptr] = this;
};;
_conchParticleTemplate2D.prototype = Object.create(WrapperObject.prototype);
_conchParticleTemplate2D.prototype.constructor = _conchParticleTemplate2D;
_conchParticleTemplate2D.prototype.__class__ = _conchParticleTemplate2D;
_conchParticleTemplate2D.__cache__ = {};
Module['_conchParticleTemplate2D'] = _conchParticleTemplate2D;

_conchParticleTemplate2D.prototype['setShader'] = _conchParticleTemplate2D.prototype.setShader = function(arg0, arg1, arg2) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  else arg1 = ensureString(arg1);
  if (arg2 && typeof arg2 === 'object') arg2 = arg2.ptr;
  else arg2 = ensureString(arg2);
  _emscripten_bind_conchParticleTemplate2D_setShader_3(self, arg0, arg1, arg2);
};;

_conchParticleTemplate2D.prototype['getID'] = _conchParticleTemplate2D.prototype.getID = function() {
  var self = this.ptr;
  return _emscripten_bind_conchParticleTemplate2D_getID_0(self);
};;

  _conchParticleTemplate2D.prototype['__destroy__'] = _conchParticleTemplate2D.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_conchParticleTemplate2D___destroy___0(self);
};
// _conchGraphics
function _conchGraphics() {
  this.ptr = _emscripten_bind_conchGraphics_conchGraphics_0();
  getCache(_conchGraphics)[this.ptr] = this;
};;
_conchGraphics.prototype = Object.create(WrapperObject.prototype);
_conchGraphics.prototype.constructor = _conchGraphics;
_conchGraphics.prototype.__class__ = _conchGraphics;
_conchGraphics.__cache__ = {};
Module['_conchGraphics'] = _conchGraphics;

_conchGraphics.prototype['getID'] = _conchGraphics.prototype.getID = function() {
  var self = this.ptr;
  return _emscripten_bind_conchGraphics_getID_0(self);
};;

  _conchGraphics.prototype['__destroy__'] = _conchGraphics.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_conchGraphics___destroy___0(self);
};
// conchNode2D
function conchNode2D() {
  this.ptr = _emscripten_bind_conchNode2D_conchNode2D_0();
  getCache(conchNode2D)[this.ptr] = this;
};;
conchNode2D.prototype = Object.create(WrapperObject.prototype);
conchNode2D.prototype.constructor = conchNode2D;
conchNode2D.prototype.__class__ = conchNode2D;
conchNode2D.__cache__ = {};
Module['conchNode2D'] = conchNode2D;

conchNode2D.prototype['getID'] = conchNode2D.prototype.getID = function() {
  var self = this.ptr;
  return _emscripten_bind_conchNode2D_getID_0(self);
};;

conchNode2D.prototype['setRootNode'] = conchNode2D.prototype.setRootNode = function() {
  var self = this.ptr;
  _emscripten_bind_conchNode2D_setRootNode_0(self);
};;

  conchNode2D.prototype['__destroy__'] = conchNode2D.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_conchNode2D___destroy___0(self);
};
// context
function context() {
  this.ptr = _emscripten_bind_context_context_0();
  getCache(context)[this.ptr] = this;
};;
context.prototype = Object.create(WrapperObject.prototype);
context.prototype.constructor = context;
context.prototype.__class__ = context;
context.__cache__ = {};
Module['context'] = context;

context.prototype['setSize'] = context.prototype.setSize = function(arg0, arg1) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  _emscripten_bind_context_setSize_2(self, arg0, arg1);
};;

context.prototype['setType'] = context.prototype.setType = function(arg0) {
  var self = this.ptr;
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  _emscripten_bind_context_setType_1(self, arg0);
};;

context.prototype['getID'] = context.prototype.getID = function() {
  var self = this.ptr;
  return _emscripten_bind_context_getID_0(self);
};;

  context.prototype['__destroy__'] = context.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_context___destroy___0(self);
};
// VoidPtr
function VoidPtr() { throw "cannot construct a VoidPtr, no constructor in IDL" }
VoidPtr.prototype = Object.create(WrapperObject.prototype);
VoidPtr.prototype.constructor = VoidPtr;
VoidPtr.prototype.__class__ = VoidPtr;
VoidPtr.__cache__ = {};
Module['VoidPtr'] = VoidPtr;

  VoidPtr.prototype['__destroy__'] = VoidPtr.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_VoidPtr___destroy___0(self);
};
// conchImage
function conchImage() {
  this.ptr = _emscripten_bind_conchImage_conchImage_0();
  getCache(conchImage)[this.ptr] = this;
};;
conchImage.prototype = Object.create(WrapperObject.prototype);
conchImage.prototype.constructor = conchImage;
conchImage.prototype.__class__ = conchImage;
conchImage.__cache__ = {};
Module['conchImage'] = conchImage;

conchImage.prototype['setImageInfo'] = conchImage.prototype.setImageInfo = function(arg0, arg1, arg2) {
  var self = this.ptr;
  ensureCache.prepare();
  if (arg0 && typeof arg0 === 'object') arg0 = arg0.ptr;
  else arg0 = ensureString(arg0);
  if (arg1 && typeof arg1 === 'object') arg1 = arg1.ptr;
  if (arg2 && typeof arg2 === 'object') arg2 = arg2.ptr;
  _emscripten_bind_conchImage_setImageInfo_3(self, arg0, arg1, arg2);
};;

conchImage.prototype['getImageID'] = conchImage.prototype.getImageID = function() {
  var self = this.ptr;
  return _emscripten_bind_conchImage_getImageID_0(self);
};;

  conchImage.prototype['__destroy__'] = conchImage.prototype.__destroy__ = function() {
  var self = this.ptr;
  _emscripten_bind_conchImage___destroy___0(self);
};
(function() {
  function setupEnums() {
    
  }
  if (Module['calledRun']) setupEnums();
  else addOnPreMain(setupEnums);
})();
