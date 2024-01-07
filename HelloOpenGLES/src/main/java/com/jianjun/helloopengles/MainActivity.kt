package com.jianjun.helloopengles

import android.content.Intent
import android.os.Bundle
import android.view.View
import androidx.appcompat.app.AppCompatActivity
import com.jianjun.helloopengles.airhockey.AirHockey2Activity
import com.jianjun.helloopengles.airhockey.AirHockeyActivity
import com.jianjun.helloopengles.clip.ClipActivity
import com.jianjun.helloopengles.databinding.ActivityMainBinding

class MainActivity : AppCompatActivity(), View.OnClickListener {

    private lateinit var binding: ActivityMainBinding

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        binding.btnTriangle.setOnClickListener(this)
        binding.btnAirHockey.setOnClickListener(this)
        binding.btnAirHockey2.setOnClickListener(this)
        binding.btnClip.setOnClickListener(this)
    }

    override fun onClick(v: View?) {
        val intent = Intent()
        when (v) {
            binding.btnTriangle -> {
                intent.setClass(this, TriangleActivity::class.java)
            }
            binding.btnAirHockey -> {
                intent.setClass(this, AirHockeyActivity::class.java)
            }
            binding.btnAirHockey2 -> {
                intent.setClass(this, AirHockey2Activity::class.java)
            }
            binding.btnClip -> {
                intent.setClass(this, ClipActivity::class.java)
            }
            else -> return
        }
        startActivity(intent)
    }
}