export class ViewRect {
  x: number
  y: number
  w: number
  h: number

  constructor(x: number, y: number, w: number, h: number) {
    this.x = x;
    this.y = y;
    this.w = w;
    this.h = h;
  }
}

export class Color4B {
  r: number
  g: number
  b: number
  a: number

  constructor(r: number, g: number, b: number, a: number) {
    this.r = r;
    this.g = g;
    this.b = b;
    this.a = a;
  }
}

export class BaseWorkerMsgEntity {
  module: string;

  function: string;

  constructor(module: string, func: string) {
    this.module = module;
    this.function = func;
  }
}

export class DialogMsgEntity extends BaseWorkerMsgEntity {
  title: string;

  message: string;

  type?:number;

  constructor(module: string, func: string) {
    super(module, func);
  }
}

export class WebViewMsgEntity extends BaseWorkerMsgEntity {
  viewTag: number

  data: string
  mimeType: string
  encoding: string
  baseURL: string

  url: string

  filePath: string

  viewRect: ViewRect

  visible: boolean

  opacity: number

  js: string

  scalesPageToFit: boolean

  constructor(module: string, func: string) {
    super(module, func);
  }
}

export class JumpMsgEntity extends BaseWorkerMsgEntity {
  url: string;

  constructor(module: string, func: string) {
    super(module, func);
  }
}