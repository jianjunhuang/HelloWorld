package xyz.juncat.helloffmpeg

import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.TextView
import xyz.juncat.helloffmpeg.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity() {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)

        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)

        // Example of a call to a native method
        binding.sampleText.text = ffmpegInfo()
    }

    /**
     * A native method that is implemented by the 'helloffmpeg' native library,
     * which is packaged with this application.
     */
    external fun stringFromJNI(): String

    external fun ffmpegInfo(): String

    companion object {
        // Used to load the 'helloffmpeg' library on application startup.
        init {
            System.loadLibrary("helloffmpeg")
        }
    }
}