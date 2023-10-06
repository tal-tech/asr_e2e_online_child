/*
 * tal_asr_api.h
 *
 *  Created on: Stempber 14, 2020
 *      Author: yanzikui
 */

#ifndef TAL_ASR_API_H_
#define TAL_ASR_API_H_

#include <string>

#define EXPORT_API __attribute__((visibility("default")))
#ifdef __cplusplus
extern "C" {
#endif

/*
 * brief ASR资源初始化
 * @param [in] resource_dir, 资源路径
 * @param [out] resource_handle, 返回句柄, 异常时候返回"NULL"
 * @param [out] 资源初始化状态, 0表示初始化成功, 其它表示异常
 */
EXPORT_API int TalASRResourceImport(const char* resource_dir,
                                    void** resource_handle);

/*
 * brief  ASR资源释放
 * @param [in] resource, ASR资源句柄
 */
EXPORT_API void TalASRResourceRelease(void* resource_handle);

/*
 * brief  获取ASR资源和SDK版本
 * @param [in] resource, ASR资源句柄
 * @param [out] ASR资源版本和SDK版本
 */
EXPORT_API std::string TalASRGetVersion(void* resource_handle);

/*
 * brief  获取ASR资源版本
 * @param [in] resource, ASR资源句柄
 * @param [out] ASR资源版本, 格式: 1.0.1
 */
EXPORT_API std::string TalASRResourceGetVersion(void* resource_handle);

/*
 * brief  获取ASR SDK版本
 * @param [out] SDK版本
 */
EXPORT_API std::string TalASRGetSDKVersion();

/*
 * brief  获取ASR资源语种
 * @param [in] resource, ASR资源句柄
 * @param [out] ASR的语种信息
 */
EXPORT_API int TalASRResourceGetLanguage(void* resource_handle);

/*
 * brief 创建一个ASR实例
 * @param [in] resource, 资源配置句柄
 * @param [out] asr_instance_handle, 识别实例句柄
 * @param [out] 状态值, 0表示创建成功, 其它表示失败
 */
EXPORT_API int TALASRInstanceCreate(void* resource_handle,
                                    void** asr_instance_handle);

/*
 * brief  ASR实例删除
 * @param [in] asr_instance, ASR实例句柄
 */
EXPORT_API void TalASRInstanceDelete(void* asr_instance_handle);

/*
 * brief  ASR实例运行设置
 * @param [in] asr_instance, ASR实例句柄
 * @param [in] params, 识别参数, json格式, 格式说明:
 * @param [out] 设置状态, 0 正常，其它异常
 */
EXPORT_API int TalASRInstanceStart(void* asr_instance_handle,
                                   const char* params);

/*
 * brief  ASR实例识别
 * @param [in] asr_instance, ASR实例句柄
 * @param [in] wav_data, wav音频数据
 * @param [in] wav_data_len, wav音频数据长度, 单位字节
 * @param [out] result, 识别结果, json格式
 * @param [out] 识别状态值, 0表示本次识别正常, 1有识别结果，-1识别异常
 */
EXPORT_API int TalASRInstanceRecognize(void* asr_instance_handle,
                                       char* wav_data, int wav_data_len,
                                       std::string& result);

/*
 * brief  ASR实例识别
 * @param [in] asr_instance, ASR实例句柄
 * @param [in] wav_data, wav音频数据
 * @param [in] wav_data_len, wav音频数据长度, 单位字节
 * @param [out] result, 识别结果, json格式
 * @param [out] 识别状态值, 0表示本次识别正常, 其它表示异常
 */
EXPORT_API int TalASRInstanceFinalRecognize(void* asr_instance_handle,
                                       char* wav_data, int wav_data_len,
                                       std::string& result);
/*
 * brief  ASR实例重置
 * @param [in] asr_instance, ASR实例句柄
 * @param [out] 状态, 0 正常，其它异常
 */
EXPORT_API int TalASRInstanceReset(void* asr_instance_handle);

#ifdef __cplusplus
};
#endif

#endif /* TAL_ASR_API_H_ */
