import hilog from '@ohos.hilog'

enum LogLevel {
  DEBUG = 0,
  INFO,
  WARN,
  ERROR
};

export class Logger {
  private domain: number
  private prefix: string
  private printLevel = LogLevel.DEBUG;

  constructor(domain: number, prefix: string) {
    this.domain = domain
    this.prefix = prefix
  }

  debug(format: string, ...args: Object[]) {
    this.log(LogLevel.DEBUG, format, ...args);
  }

  info(format: string, ...args: Object[]) {
    this.log(LogLevel.INFO, format, ...args);
  }

  warn(format: string, ...args: Object[]) {
    this.log(LogLevel.WARN, format, ...args);
  }

  error(format: string, ...args: Object[]) {
    this.log(LogLevel.ERROR, format, ...args);
  }

  log(logLevel: LogLevel, format: string, ...args: Object[]) {
    switch (logLevel) {
      case LogLevel.DEBUG:
        if (this.printLevel > LogLevel.DEBUG) {
          return;
        }
        hilog.debug(this.domain, this.prefix, format, args)
        break;
      case LogLevel.INFO:
        if (this.printLevel > LogLevel.INFO) {
          return;
        }
        hilog.info(this.domain, this.prefix, format, args)
        break;
      case LogLevel.WARN:
        if (this.printLevel > LogLevel.WARN) {
          return;
        }
        hilog.warn(this.domain, this.prefix, format, args)
        break;
      case LogLevel.ERROR:
        if (this.printLevel > LogLevel.ERROR) {
          return;
        }
        hilog.error(this.domain, this.prefix, format, args)
        break;
    }
  }
}