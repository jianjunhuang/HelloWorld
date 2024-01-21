package xyz.juncat.hellojni

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.TextView
import xyz.juncat.hellojni.databinding.ActivityMainBinding
import xyz.juncat.jni.lib.Account
import xyz.juncat.jni.lib.HelloJNI

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        val helloJNI = HelloJNI()
        binding.sampleText.text = helloJNI.dynamicStringFromJNI()

        helloJNI.stringInNative("demo for string in native")
        helloJNI.referenceInJNI(arrayOf<String>(
            "1", "2", "3", "4", "5"
        ))
        val account = Account(1, "1", "2")
        helloJNI.accessAccount(account)
        Log.i(TAG, "accessAccount: $account")
        helloJNI.accessStaticFiled(account)
        Log.i(TAG, "accessAccount static: ${Account.staticId}")

        //JNI access Java method
        helloJNI.accessAccountMethod(account)
        helloJNI.accessAccountMethodWithArg(account)
        Log.i(TAG, "accessAccountMethodWithArg: $account")
        helloJNI.callbackFromJNI(object :HelloJNI.Callback {
            override fun onCall() {
                Log.i(TAG, "onCall from JNI")
            }

        })
    }

    companion object {
        private const val TAG = "MainActivity"
    }
}