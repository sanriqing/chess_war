import { IS_DEV } from '~/constants/index';
/**
 *
 * 沉睡一段时间
 * @export
 * @param {number} [time=1000]
 * @returns promise
 */
export function sleep(time = 1000) {
    return new Promise((resolve) => {
        setTimeout(resolve, time);
    });
}

/**
 *
 * 超时报错
 * @export
 * @param {Error} [error=new Error()] 错误
 * @param {number} [time=3000] 时间
 */
export async function sleepAndThrowError(error = new Error('time out'), time = 3000) {
    await sleep(time);
    throw error;
}

export function warn(...args: any[]) {
    if (!IS_DEV) {
        console.warn('【 warning 】', ...args);
    }
}

export function noop(...args: any[]) { }
