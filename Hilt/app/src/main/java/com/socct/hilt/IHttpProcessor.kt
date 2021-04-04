package com.socct.hilt


interface IHttpProcessor {


    fun post(url: String, params: Map<String, Any>, callback: ICallback)

    fun get(url: String, params: Map<String, Any>, callback: ICallback)

}