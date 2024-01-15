package xyz.juncat.hellojni

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import xyz.juncat.hellojni.databinding.ActivityMainBinding
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
    }

}