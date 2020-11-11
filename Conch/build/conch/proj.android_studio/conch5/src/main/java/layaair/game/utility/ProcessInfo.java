package layaair.game.utility;

import java.io.BufferedReader;
import java.io.FileInputStream;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import android.annotation.SuppressLint;
import android.app.ActivityManager;
import android.app.ActivityManager.MemoryInfo;
import android.os.Build;
import android.os.Debug;
import android.util.Log;

public class ProcessInfo 
{
	static long mnLastTotalCpu=0;
	static long mnLastAppCpu=0;
	static String TAG = "PROCESSINFO";
	static ActivityManager sAM = null;	
	static long mWorkCpuTm = 0;
	static long mLastWorkCpuTm = 0;
	static public void init( ActivityManager pAM )
	{
		 sAM = pAM;
	}
	static public void uninit(){
		sAM = null;
	}
	
	//16之前的
	static public long getTotalMemoryOld() {  
		String str1 = "/proc/meminfo";
		String str2;        
		String[] arrayOfString;
		long initial_memory = 0;
		try {
		FileReader localFileReader = new FileReader(str1);
		BufferedReader localBufferedReader = new BufferedReader(    localFileReader, 8192);
		str2 = localBufferedReader.readLine();//meminfo
		arrayOfString = str2.split("\\s+");
		for (String num : arrayOfString) {
		Log.i(str2, num + "\t");
		}
		//total Memory
		initial_memory = Integer.valueOf(arrayOfString[1]).intValue();// * 1024;   
		localBufferedReader.close();
		return initial_memory;
		} 
		catch (IOException e) 
		{       
		    return -1;
		}
	}  	
	@SuppressLint("NewApi")
	static public float getTotalMem()
	{
		if( Build.VERSION.SDK_INT<16 ){
			return (float)getTotalMemoryOld();
		}
		
		if(sAM == null ) return 0;
		MemoryInfo meminfo = new MemoryInfo();
		sAM.getMemoryInfo(meminfo);
		float total = meminfo.totalMem/1024.0f;
		Log.i("", "total:"+total);
		return total;
	}
	
	static public float getAvalidMem()
	{
		if(sAM == null ) return 0;
		MemoryInfo meminfo = new MemoryInfo();
		sAM.getMemoryInfo(meminfo);
		float availmem = meminfo.availMem/1024.0f;
		return availmem;
	}
	static public float getUsedMem()
	{
		if( sAM ==null )return 0;
		int pid = android.os.Process.myPid();
		Debug.MemoryInfo[] memInfos = sAM.getProcessMemoryInfo(new int[] {pid});
		float usedMem = (float)memInfos[0].getTotalPrivateDirty();
        Log.d(TAG, "占用内存：" + usedMem );
        return usedMem;
    }
	
	static public int GetFPS()
	{
		return 0;
	}
	
	//得到当前进程的cpu占用率
	public static float getProcessCpuRate1()
	{
		long curTotal = getTotalCpuTime();
		long curApp = getAppCpuTime();
		long dapp = curApp-mnLastAppCpu;
		long dtotal = curTotal-mnLastTotalCpu;
		float cpuRate = (float) (dapp*1.0/dtotal);
		if(cpuRate>1)
			Log.e("","app:"+dapp+",total:"+dtotal);
		mnLastAppCpu = curApp;
		mnLastTotalCpu = curTotal;
		return cpuRate;
	}
	public static float getProcessCpuRate()
	{
		long curTotal = getTotalCpuTime();
		long dwork = mWorkCpuTm-mLastWorkCpuTm;
		long dtotal = curTotal-mnLastTotalCpu;
		float cpuRate = (float) (dwork*1.0/dtotal);
		mLastWorkCpuTm=mWorkCpuTm;
		mnLastTotalCpu = curTotal;
		return cpuRate;
	}
    
	/*
	 * 判断是否支持neon
	 */
	public static boolean supportNeon(){
		try
		{
			BufferedReader reader = new BufferedReader(new InputStreamReader(
				new FileInputStream("/proc/cpuinfo")), 1000);
			String li = reader.readLine();
				while(li!=null){
				if( li.startsWith("Features")){
					if( li.contains("neon")){
						return true;
					}else{
						return false;
					}
				}
				li = reader.readLine();
			}
			reader.close();
		}
		catch (IOException ex)
		{
			ex.printStackTrace();
		}
		return false;
	}
	
	public static long getTotalCpuTime()
	{ 
		// 获取系统总CPU使用时间
		String[] cpuInfos = null;
		try
		{
           BufferedReader reader = new BufferedReader(new InputStreamReader(
                   new FileInputStream("/proc/stat")), 1000);
           String load = reader.readLine();
           reader.close();
           cpuInfos = load.split(" ");
		}
		catch (IOException ex)
		{
			ex.printStackTrace();
		}
		
		long worktm = Long.parseLong(cpuInfos[2])  //user mode cpu time
				+ Long.parseLong(cpuInfos[3]) //nice <0 cpu time
				+ Long.parseLong(cpuInfos[4])	//system:内核时间
				+ Long.parseLong(cpuInfos[7]) //irq:
				+ Long.parseLong(cpuInfos[8]);//softirq
		
		long waittm = Long.parseLong(cpuInfos[6]) //idle:除了iowait外的空闲时间
				+ Long.parseLong(cpuInfos[5]);	//iowait:
		
		mWorkCpuTm=worktm;
		/*
      long totalCpu = Long.parseLong(cpuInfos[2])  //user mode cpu time
               + Long.parseLong(cpuInfos[3]) //nice <0 cpu time
               + Long.parseLong(cpuInfos[4])	//system:内核时间
               + Long.parseLong(cpuInfos[6]) //idle:除了iowait外的空闲时间
               + Long.parseLong(cpuInfos[5])	//iowait:
               + Long.parseLong(cpuInfos[7]) //irq:
               + Long.parseLong(cpuInfos[8]);//softirq
        */
      return worktm+waittm;
   }
    
	public static long getAppCpuTime()
	{ 
		// 获取应用占用的CPU时间
		String[] cpuInfos = null;
		try
		{
			int pid = android.os.Process.myPid();
			BufferedReader reader = new BufferedReader(new InputStreamReader(
                   new FileInputStream("/proc/" + pid + "/stat")), 1000);
			String load = reader.readLine();
			reader.close();
			cpuInfos = load.split(" ");
		}
		catch (IOException ex)
		{
			ex.printStackTrace();
		}
		long appCpuTime = Long.parseLong(cpuInfos[13])
               + Long.parseLong(cpuInfos[14]) + Long.parseLong(cpuInfos[15])
               + Long.parseLong(cpuInfos[16]);
		return appCpuTime;
   }	
}
