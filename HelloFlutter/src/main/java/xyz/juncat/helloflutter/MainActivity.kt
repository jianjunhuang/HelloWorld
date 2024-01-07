package xyz.juncat.helloflutter

import android.os.Bundle
import android.widget.Button
import androidx.appcompat.app.AppCompatActivity
import androidx.fragment.app.FragmentManager
import io.flutter.embedding.android.FlutterActivity
import io.flutter.embedding.android.FlutterActivityLaunchConfigs
import io.flutter.embedding.android.FlutterFragment
import io.flutter.embedding.android.RenderMode
import io.flutter.embedding.engine.FlutterEngine
import io.flutter.embedding.engine.FlutterEngineCache
import io.flutter.embedding.engine.dart.DartExecutor


class MainActivity : AppCompatActivity() {

    private val flutterEngineId = "pre-warm-engine"

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)
        //pre-warm FlutterEngine
        val flutterEngine = FlutterEngine(this)
        flutterEngine.dartExecutor.executeDartEntrypoint(DartExecutor.DartEntrypoint.createDefault())
        FlutterEngineCache
            .getInstance()
            .put(flutterEngineId, flutterEngine)

        findViewById<Button>(R.id.btn_show_flutter_warm).setOnClickListener {
            val intent = FlutterActivity
                .withCachedEngine(flutterEngineId)
                .build(this)
            startActivity(intent)
        }
        findViewById<Button>(R.id.btn_show_flutter).setOnClickListener {
            val intent = FlutterActivity
                .withNewEngine()
                .backgroundMode(FlutterActivityLaunchConfigs.BackgroundMode.transparent)
                .initialRoute("/my_route")
                .build(this)
            startActivity(intent)
        }


        //init
        val fragmentManager: FragmentManager = supportFragmentManager

        // Attempt to find an existing FlutterFragment,
        // in case this is not the first time that onCreate() was run.

        var flutterFragment = fragmentManager
            .findFragmentByTag(TAG_FLUTTER_FRAGMENT) as? FlutterFragment

        if (flutterFragment == null) {
            flutterFragment = FlutterFragment.withCachedEngine(flutterEngineId)
                .build()
            fragmentManager
                .beginTransaction()
                .add(
                    R.id.fl_container,
                    flutterFragment,
                    TAG_FLUTTER_FRAGMENT
                ).commit()
        }
    }

    companion object {
        const val TAG_FLUTTER_FRAGMENT = "tag_flutter_fragment"
    }
}