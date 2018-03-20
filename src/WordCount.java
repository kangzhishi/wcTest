import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.BufferedOutputStream;
import java.io.FilenameFilter;


import java.util.regex.Pattern;


public class wcTest {
	
	
	private static int index(Command command,Command[] commands){
		for(int i=0;i<commands.length;i++)
			if(command.equals(commands[i]))
				return i;
		return -1;
	}
	private static int select(Command[] commands){
		for(int i=1;i<commands.length;i++)
			if(commands[i]==null&&commands[i-1]!=null)
				return i;
		return -1;
	}

	private static void execute(Command command,FileInfo fileInfo,PrintWriter ars){
		switch (command) {
		case c:ars.write(fileInfo.path()+", 字符数: "+fileInfo.cNum()+"\r\n");break;
		case w:ars.write(fileInfo.path()+", 单词数: "+fileInfo.wNum()+"\r\n");break;
		case line:ars.write(fileInfo.path()+", 行   数: "+fileInfo.lineNum()+"\r\n");break;
		case ALL:fileInfo.toFile(ars);break;
		}
	}
	
	private static Command analysisCommand(String command){
		if(command.equals("-c"))
			return Command.c;
		else if(command.equals("-w"))
			return Command.w;
		else if(command.equals("-l"))
			return Command.line;
		else if(command.equals("-o"))
			return Command.output;
		else if(command.equals("-s"))
			return Command.SALL;
		else if(command.equals("-a"))
			return Command.ALL;
		else if(command.equals("-e"))
			return Command.EXCEPT;
		else return null;
	}
	public static void main(String[] args) throws FileNotFoundException{
		String[] par=args;
		int length=par.length;
		FileInfo fileInfo=null;
		PrintWriter ars=new PrintWriter(new BufferedOutputStream(new FileOutputStream("result.txt",true)));
		
		if(length==2){
			Command command=analysisCommand(args[0]);
			fileInfo=new FileInfo(Paths.get(args[1]));
			execute(command, fileInfo, ars);
			ars.flush();
		}
		else if(length>2){
			Command[] commands=new Command[length];
			for(int i=0;i<length;i++)
				commands[i]=analysisCommand(args[i]);
			int index;
			if((index=index(Command.output, commands))!=-1){
				ars=new PrintWriter(new BufferedOutputStream(new FileOutputStream(args[index+1],true)));	
				commands[index]=null;
			}
			String name=par[select(commands)];
			if((index=index(Command.SALL, commands))==-1){
				fileInfo=new FileInfo(Paths.get(name));
				for(Command command:commands)
					if(command!=null)
						execute(command, fileInfo, ars);
			}else{
				commands[index]=null;
				String[] list=new File("").getAbsoluteFile().list(new DirFilter(name));
				for(String item:list){
					FileInfo fileInfo2=new FileInfo(Paths.get(item));
					for(Command command:commands)
						if(command!=null)
							execute(command, fileInfo2, ars);
				}
			}
			ars.flush();
				
		}
		ars.close();
	}
}


class FileInfo{
	private int cNum;
	private int wNum;
	private int lineSum;
	private int spaceSum;
	private int codeSum;
	private int noteSum;
	private Path path;
	private boolean isAnalysis=false;
	public FileInfo(Path path){
		this.path=path;
		cNum=0;
		wNum=0;
		lineSum=0;
		spaceSum=0;
		codeSum=0;
		noteSum=0;
		analysis();
	}
	
	public String path(){
		return path.toString();
	}
	
	public int cNum(){
		return cNum;
	}
	public int wNum(){
		return wNum;
	}
	public int lineNum(){
		return lineSum;
	}
	
	
	private void analysis(){
		if(!isAnalysis){
			isAnalysis=true;
			String s;
			BufferedReader in = null;
			try {
				in =new BufferedReader(new FileReader(path.toString()));
				while((s=in.readLine())!=null){
					switch(analysisline(s)){
					case SPACE:spaceSum++;break;
					case CODE:codeSum++;break;
					case NOTE:noteSum++;break;
					}
					cNum+=s.replaceAll("\\s", "").length();
					wNum+=s.split("\\W+").length;
					//sb.append(s);
					lineSum++;
				}
				
			} catch (IOException e) {
				System.out.println("文件不存在"+path());
			}finally{
				try {
					in.close();
				} catch (IOException e) {
					// TODO Auto-generated catch block
					e.printStackTrace();
				}
			}
		}
	}
	public void toFile(PrintWriter ars){
		ars.write(path.toString()+", 字符数: "+cNum+"\r\n"
				+path.toString()+", 单词数: "+wNum+"\r\n"
				+path.toString()+", 行数: "+lineSum+"\r\n"
				+path.toString()+", 代码行/空行/注释行: "+codeSum+"/"+spaceSum+"/"+noteSum+"\r\n");
	}
	public String toString(){
		return path.toString()+"cNum="+cNum+" wNum="+wNum+" lineSum="+lineSum+" codeSum="+codeSum+" noteSum="+noteSum+" spaceSum="+spaceSum;
	}
	
	//代码行/空行/注释行/
	private Type analysisline(String line){
		return (line.equals("")||Pattern.matches("\\s+", line))?Type.SPACE:(Pattern.matches("//.*", line)?Type.NOTE:Type.CODE);
	}
}

enum Command{
	c,w,line,ALL,output,SALL,EXCEPT;
}

enum Type{
	SPACE,CODE,NOTE
}

class DirFilter implements FilenameFilter{
	private Pattern pattern;
	public DirFilter(String regex){
		pattern = Pattern.compile(regex);
	}
	public boolean accept(File dir,String name){
		return pattern.matcher(name).matches();
	}
}
