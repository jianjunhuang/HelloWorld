package xyz.juncat.helloaidl

import android.app.Service
import android.content.ComponentCallbacks
import android.content.Intent
import android.os.IBinder
import xyz.juncat.lib.aidl.IMediaInterface

class RemoteService : Service() {

    private val binder = object : IMediaInterface.Stub() {
        override fun basicTypes(
            anInt: Int,
            aLong: Long,
            aBoolean: Boolean,
            aFloat: Float,
            aDouble: Double,
            aString: String?
        ) {

        }

    }
    override fun onCreate() {
        super.onCreate()
    }

    override fun onBind(intent: Intent?): IBinder? {
        return binder
    }

}