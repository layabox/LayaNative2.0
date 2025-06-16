interface ResultMsg {
  errCode: number;
  errMsg: string;
  data?: Object;
}
export class Result {
  public static success(data?: Object): ResultMsg {
    return {
      "errCode": 0,
      "errMsg": "",
      "data": data,
    };
  }

  public static error(errCode: number, errMsg: string): ResultMsg {
    return {
      "errCode": errCode,
      "errMsg": errMsg,
    };
  }
};