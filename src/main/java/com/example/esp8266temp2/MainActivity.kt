package com.example.esp8266temp2

import android.content.SharedPreferences
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.view.Menu
import android.view.MenuItem
import android.view.View
import com.example.esp8266temp2.databinding.ActivityMainBinding
import okhttp3.OkHttpClient
import okhttp3.Request
import java.io.IOException

class MainActivity : AppCompatActivity() {
    private lateinit var request: Request
    private lateinit var binding: ActivityMainBinding
    private lateinit var pref: SharedPreferences
    private  val client = OkHttpClient()

    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        binding = ActivityMainBinding.inflate(layoutInflater)
        setContentView(binding.root)
        pref = getSharedPreferences("MyPref", MODE_PRIVATE)
       onClickSaveIP()
        getIp()
//        binding.apply {
//            bLed1.setOnClickListener(onClickListeer())
//            bLed2.setOnClickListener(onClickListeer())
//            bLed3.setOnClickListener(onClickListeer())
//        }
    }

    // override fun onCreateOptionsMenu(menu: Menu?): Boolean {
       // menuInflater.inflate(R.menu.main_menu, menu)
       // return true
    // }

    // override fun onOptionsItemSelected(item: MenuItem): Boolean {
       // if(item.itemId == R.id.sync) post("temperature")
       // return true
    // }

    private fun onClickListeer(): View.OnClickListener{
        return View.OnClickListener {
//            when(it.id){
//                R.id.bLed1 > { post("led1") }
//                        R.id.bLed1 > { post("led2") }
//                        R.id.bLed1 > { post("led3") }
//            }
        }
    }

    private fun getIp() = with(binding){
        val ip = pref.getString("ip", "")
        if(ip != null){
            if(ip.isNotEmpty()) edip.setText(ip)
        }
    }

    private fun onClickSaveIP() = with(binding){
        buttonSave.setOnClickListener {
            if(edip.text.isNotEmpty())saveIp(edip.text.toString())
        }
    }

    private fun saveIp(ip: String){
        val editor = pref.edit()
        editor.putString("ip",ip)
        editor.apply()
    }

    private fun post(post: String) {
        Thread{

            request = Request.Builder().url("http://${binding.edip.text}/$post").build()
            try {
                var response = client.newCall(request).execute()
                if(response.isSuccessful){
                    val resultText = response.body()?.string()
                    runOnUiThread {
                        val temp = resultText + "C°"
                        binding.tvTemp.text = temp
                    }
                }
            } catch (i: IOException){

            }


        }.start()
    }
}