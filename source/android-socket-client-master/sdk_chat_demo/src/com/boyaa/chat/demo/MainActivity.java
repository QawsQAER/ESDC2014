package com.boyaa.chat.demo;
	import android.app.Activity;  

import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;  
import java.net.URLConnection;  
import java.io.IOException;
import java.io.InputStream;  

	
import java.io.InputStream;
import java.net.HttpURLConnection;
import java.net.MalformedURLException;
import java.net.URL;

import android.app.Activity;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.widget.ImageView;





	import android.os.Bundle;  
import android.os.Environment;  
import android.widget.MediaController;  
import android.widget.VideoView;  
import android.widget.ImageView;  
import android.app.Activity;
import android.util.Log;
import android.view.KeyEvent;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.EditText;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;







import android.graphics.drawable.Drawable;

import com.boyaa.chat.R;
import com.boyaa.push.lib.service.Client;
import com.boyaa.push.lib.service.ISocketResponse;
import com.boyaa.push.lib.service.Packet;

public class MainActivity extends Activity {

	private Client user=null;
	private EditText ip,port,recContent;
	
	@Override
	protected void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.activity_main);
		
		
		ImageView imageView;
	    imageView = (ImageView)findViewById(R.id.image_view);
	    
	    String urlStr = "http://www.cse.cuhk.edu.hk/v7/images/about/leader/hermanlam.jpg";
	  
	 Bitmap bitmap = getHttpBitmap(urlStr);
        //显示
     imageView.setImageBitmap(bitmap);
     
		initView();
		user=new Client(this.getApplicationContext(),socketListener);
		
		
		
	}
	


	
	private void initView()
	{
		
		findViewById(R.id.open).setOnClickListener(listener);
		findViewById(R.id.Up).setOnClickListener(listener);
		findViewById(R.id.Down).setOnClickListener(listener);
		findViewById(R.id.Left).setOnClickListener(listener);
		findViewById(R.id.Right).setOnClickListener(listener);
		
		findViewById(R.id.cUp).setOnClickListener(listener);
		findViewById(R.id.cDown).setOnClickListener(listener);
		findViewById(R.id.cLeft).setOnClickListener(listener);
		findViewById(R.id.cRight).setOnClickListener(listener);
		
//		findViewById(R.id.clear).setOnClickListener(listener);
		
		ip=(EditText) findViewById(R.id.ip);
		port=(EditText) findViewById(R.id.port);
//		sendContent=(EditText) findViewById(R.id.sendContent);
		recContent=(EditText) findViewById(R.id.recContent);
		
		ip.setText("192.168.210.239");
		port.setText("60000");

		
		
		
	}
	
	
	public static Bitmap getHttpBitmap(String url){
    	URL myFileURL;
    	Bitmap bitmap=null;
    	try{
    		myFileURL = new URL(url);
    		//获得连接
    		HttpURLConnection conn=(HttpURLConnection)myFileURL.openConnection();
    		//设置超时时间为6000毫秒，conn.setConnectionTiem(0);表示没有时间限制
    		conn.setConnectTimeout(6000);
    		//连接设置获得数据流
    		conn.setDoInput(true);
    		//不使用缓存
    		conn.setUseCaches(false);
    		//这句可有可无，没有影响
    		//conn.connect();
    		//得到数据流
    		InputStream is = conn.getInputStream();
    		//解析得到图片
    		bitmap = BitmapFactory.decodeStream(is);
    		//关闭数据流
    		is.close();
    	}catch(Exception e){
    		e.printStackTrace();
    	}
    	
		return bitmap;
    	
    }
	
	
	
	
	
	private ISocketResponse socketListener=new ISocketResponse() {
		
		@Override
		public void onSocketResponse(final String txt) {
			
			if(txt==)
			
			/*runOnUiThread(new Runnable() {
				public void run() {
					recContent.getText().append(txt).append("");
				}
			});*/
		}
	};
	
	private OnClickListener listener=new OnClickListener() {
		Packet packet=new Packet();
		@Override
		public void onClick(View v) {
			switch(v.getId())
			{
			
				case R.id.open:
//					user.open();
					user.open(ip.getText().toString(), Integer.valueOf(port.getText().toString()));
					packet.pack("cr");
					user.send(packet);
					break;
		
					
				case R.id.Up:
//					packet.pack(sendContent.getText().toString());
					packet.pack("Up");
					user.send(packet);
//					sendContent.setText("");
					break;
					
					
					
				case R.id.Left:
					packet.pack("Le");
					user.send(packet);
					break;
					
				case R.id.Right:
					packet.pack("Ri");
					user.send(packet);
					break;
					
				case R.id.Down:
					packet.pack("Do");
					user.send(packet);
					break;
					
				case R.id.cUp:
//					packet.pack(sendContent.getText().toString());
					packet.pack("cU");
					user.send(packet);
//					sendContent.setText("");
					break;
					
					
					
				case R.id.cLeft:
					packet.pack("cL");
					user.send(packet);
					break;
					
				case R.id.cRight:
					packet.pack("cR");
					user.send(packet);
					break;
					
				case R.id.cDown:
					
					packet.pack("cD");
					user.send(packet);
					break;
					
//				case R.id.clear:
//					recContent.setText("");
//					break;
			}
		}
	};
	
	@Override
	public boolean onKeyDown(int keyCode, KeyEvent event) {
		if(keyCode==KeyEvent.KEYCODE_BACK)
		{
			android.os.Process.killProcess(android.os.Process.myPid());
		}
		return super.onKeyDown(keyCode, event);
	}
	
	
	
	
	
	
}
