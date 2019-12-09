// 获取第一个参数的类型
export type GetFirstParamType<T extends (arg: any, ...args: any[]) => any> = T extends (arg: infer R, ...args: any) => any ? R : never
// 获取构造函数类型
export type GetConstruction<T> = new (...args: any[]) => T;
// 简单的字典类型
export type SimpleStringObject = {
    [k: string]: string
}
// 取消所有只读
export type NotReadOnly<T extends object> = {
    -readonly [K in keyof T]: T[K]
};
