package com.socct.hilt

import java.lang.reflect.ParameterizedType

abstract class HttpCallback<T> : ICallback {


    override fun onSuccess(str: String) {
        val type = this::class.java.genericSuperclass as ParameterizedType

        val params = type.actualTypeArguments
        val clazz = params[0] as Class<T>
        onSuccess(clazz.newInstance())
    }

    abstract fun onSuccess(t: T);

    override fun onError() {

    }
}