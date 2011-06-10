package com.maxconncoll.androidcamera;
import java.io.FileNotFoundException;
import java.io.IOException;
import java.io.OutputStream;

import android.app.Activity;
import android.content.ContentValues;
import android.content.pm.ActivityInfo;
import android.graphics.PixelFormat;
import android.hardware.Camera;
import android.hardware.Camera.PictureCallback;
import android.hardware.Camera.ShutterCallback;
import android.net.Uri;
import android.os.Bundle;
import android.provider.MediaStore.Images.Media;
import android.view.LayoutInflater;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.Button;
import android.widget.Toast;

public class AndroidCamera extends Activity implements SurfaceHolder.Callback{
	public Uri uriTarget;
	Camera camera;
	SurfaceView surfaceView;
	SurfaceHolder surfaceHolder;
	boolean previewing = false;
	LayoutInflater controlInflater = null;
	
	/** Called when the activity is first created. */
	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);
    
		getWindow().setFormat(PixelFormat.UNKNOWN);
		surfaceView = (SurfaceView)findViewById(R.id.camerapreview);
		surfaceHolder = surfaceView.getHolder();
		surfaceHolder.addCallback(this);
		surfaceHolder.setType(SurfaceHolder.SURFACE_TYPE_PUSH_BUFFERS);
    
		controlInflater = LayoutInflater.from(getBaseContext());
		View viewControl = controlInflater.inflate(R.layout.control, null);
		LayoutParams layoutParamsControl = new LayoutParams(LayoutParams.FILL_PARENT,LayoutParams.FILL_PARENT);
		this.addContentView(viewControl, layoutParamsControl);
    
		Button buttonTakePicture = (Button)findViewById(R.id.takepicture);
		//Button buttonLoadPicture = (Button)findViewById(R.id.editpicture);
		buttonTakePicture.setOnClickListener(new Button.OnClickListener(){

			@Override
			public void onClick(View arg0) {
				// 	TODO Auto-generated method stub
				camera.takePicture(myShutterCallback,myPictureCallback_RAW, myPictureCallback_JPG);
			}});
		//buttonLoadPicture.setOnClickListener(new Button.OnClickListener(){
			//@Override
			//public void onClick(View arg0){
				//Toast.makeText(androidcamera.this,"I'll be with you in a second, Sir.",Toast.LENGTH_LONG).show();
				//Intent send = new Intent(androidcamera.this, Send.class);
				//startActivity(send);
				
				
				
			//}});
		}
	
	ShutterCallback myShutterCallback = new ShutterCallback(){

		@Override
		public void onShutter() {
			// TODO Auto-generated method stub
 
		}};

		PictureCallback myPictureCallback_RAW = new PictureCallback(){

			@Override
			public void onPictureTaken(byte[] arg0, Camera arg1) {
				// TODO Auto-generated method stub
 
			}};

			PictureCallback myPictureCallback_JPG = new PictureCallback(){

				@Override
				public void onPictureTaken(byte[] arg0, Camera arg1) {
					// TODO Auto-generated method stub
					uriTarget = getContentResolver().insert(Media.EXTERNAL_CONTENT_URI,new ContentValues());
					OutputStream imageFileOS;
					
					try{
						imageFileOS = getContentResolver().openOutputStream(uriTarget);
						imageFileOS.write(arg0);
						imageFileOS.flush();
						imageFileOS.close();
						
						/*AlertDialog.Builder builder = new AlertDialog.Builder(androidcamera.this);
						builder.setMessage("Would you like to send this picture?")
							.setCancelable(false)
							.setPositiveButton("Yes",new DialogInterface.OnClickListener(){
								public void onClick(DialogInterface dialog, int id){
									//Intent send = new Intent(androidcamera.this, Send.class);
									//startActivity(send);
									send test = new send();
									test.Send();
								}
							})
							.setNegativeButton("No", new DialogInterface.OnClickListener() {
							
								public void onClick(DialogInterface dialog, int which) {
									dialog.cancel();
								
								}
							});
						builder.create().show();*/
						Toast.makeText(androidcamera.this, "Image Saved: "+uriTarget.toString(),Toast.LENGTH_LONG).show();
						
						//Bitmap test = createBitmap();
						//OutputStream stream = new FileOutputStream("/sdcard/test.png");
						//test.compress(CompressFormat.JPEG, 80, stream);
						
						
					}catch(FileNotFoundException e){
						//TODO Auto-generated catch block
						e.printStackTrace();
					}catch(IOException e){
						//TODO Auto-generated catch block
						e.printStackTrace();
					}
					camera.startPreview();
				}};

				@Override
				public void surfaceChanged(SurfaceHolder holder, int format, int width,int height) {
					// TODO Auto-generated method stub
					if(previewing){
						camera.stopPreview();
						previewing = false;
					}

					if (camera != null){
						try {
							camera.setPreviewDisplay(surfaceHolder);
							camera.startPreview();
							previewing = true;
						} catch (IOException e) {
							// TODO Auto-generated catch block
							e.printStackTrace();
						}
					}
				}

				@Override
				public void surfaceCreated(SurfaceHolder holder) {
					// TODO Auto-generated method stub
					camera = Camera.open();
				}

				@Override
				public void surfaceDestroyed(SurfaceHolder holder) {
					// TODO Auto-generated method stub
					camera.stopPreview();
					camera.release();
					camera = null;
					previewing = false;
				}
				/*public class send {
					public void Send(ProfileVO pvo){
						Log.i(getClass().getSimpleName(),"send task - start");
						HttpParams p = new BasicHttpParams();
						p.setParameter("test", pvo.getName());
						
						HttpClient client = new DefaultHttpClient(p);
						try{
							HttpResponse response = client.execute(new HttpGet(""));
							InputStream is = response.getEntity().getContent();
						}catch (ClientProtocolException e){
							e.printStackTrace();
						}catch (IOException e){
							e.printStackTrace();
						}
						Log.i(getClass().getSimpleName(),"send task - end");
					}
				}*/
}

