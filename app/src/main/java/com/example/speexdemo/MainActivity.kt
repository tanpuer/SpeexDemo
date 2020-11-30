package com.example.speexdemo

import android.Manifest
import android.content.pm.PackageManager
import android.media.*
import android.os.Build
import androidx.appcompat.app.AppCompatActivity
import android.os.Bundle
import android.widget.Toast
import androidx.core.app.ActivityCompat
import androidx.core.content.ContextCompat
import java.io.IOException

class MainActivity : AppCompatActivity() {

    private lateinit var mAudioRecord: AudioRecord
    private lateinit var mAudioTrack: AudioTrack
    private var writeThread: Thread? = null


    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        if (Build.VERSION.SDK_INT >= 21 && ContextCompat.checkSelfPermission(
                this,
                Manifest.permission.RECORD_AUDIO
            ) != PackageManager.PERMISSION_GRANTED
        ) {
            ActivityCompat.requestPermissions(
                this,
                arrayOf(Manifest.permission.RECORD_AUDIO),
                10001
            )
        } else {
            initAudio()
        }
    }

    override fun onRequestPermissionsResult(
        requestCode: Int,
        permissions: Array<out String>,
        grantResults: IntArray
    ) {
        if (requestCode == 10001 && grantResults.isNotEmpty() && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            initAudio()
        } else {
            Toast.makeText(this, "must need audio record permission!", Toast.LENGTH_LONG)
                .show()
        }
    }

    private fun initAudio() {
        nativeInitSpeex(DEFAULT_BUFFER_SIZE, 44100)
        initAudioTrack()
        initAudioRecord()
    }

    private fun initAudioTrack() {
        val minBufferSize = AudioTrack.getMinBufferSize(
            44100,
            AudioFormat.CHANNEL_OUT_MONO,
            AudioFormat.ENCODING_PCM_16BIT
        )
        mAudioTrack = AudioTrack(
            AudioManager.STREAM_MUSIC,
            44100,
            AudioFormat.CHANNEL_OUT_MONO,
            AudioFormat.ENCODING_PCM_16BIT,
            minBufferSize,
            AudioTrack.MODE_STREAM
        )
    }

    private fun initAudioRecord() {
        val minBufferSize = AudioRecord.getMinBufferSize(44100, 1, AudioFormat.ENCODING_PCM_16BIT)
        mAudioRecord = AudioRecord(
            MediaRecorder.AudioSource.MIC,
            44100,
            1,
            AudioFormat.ENCODING_PCM_16BIT,
            minBufferSize
        )
        writeThread = object : Thread() {
            override fun run() {
                try {
                    mAudioRecord.startRecording()
                    val buffer = ShortArray(minBufferSize)
                    while (mAudioRecord.recordingState == AudioRecord.RECORDSTATE_RECORDING) {
                        mAudioRecord.read(buffer, 0, minBufferSize)

                        val result = nativeSpeexProcessFrame(buffer)

                        mAudioTrack.write(result, 0, result.size)
                    }
                    writeThread?.join()
                } catch (e: IOException) {
                    e.printStackTrace()
                }
            }
        }
        writeThread?.start()
        mAudioTrack.play()
    }

    external fun nativeInitSpeex(frameSize: Int, sampleRate: Int)
    external fun nativeSpeexProcessFrame(shortArray: ShortArray): ShortArray
    external fun nativeReleaseSpeex()

    companion object {
        init {
            System.loadLibrary("native-lib")
        }
    }
}