package WordTest;

import java.io.BufferedOutputStream;
import java.io.BufferedReader;
import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.FileReader;
import java.io.FilenameFilter;
import java.io.IOException;
import java.io.PrintWriter;
import java.nio.file.Path;
import java.nio.file.Paths;

import java.util.regex.Pattern;


public class WordCount {
	
	public static void main(String[] args1) throws FileNotFoundException{
		String[] args={"-a","-s",".*java"};
		String[] param=args;
		int length=param.length;
		FileInfo fileInfo=null;
		PrintWriter pw=new PrintWriter(new BufferedOutputStream(new FileOutputStream("result.txt",true)));
		if(length==2){
			Command command=analysisCommand(args[0]);
			fileInfo=new FileInfo(Paths.get(args[1]));
			execute(command, fileInfo, pw);
			pw.flush();
		}
		else if(length>2){
			Command[] commands=new Command[length];
			for(int i=0;i<length;i++)
				commands[i]=analysisCommand(args[i]);
			int index;
			if((index=indexOf(Command.OUTPUT, commands))!=-1){
				pw=new PrintWriter(new BufferedOutputStream(new FileOutputStream(args[index+1],true)));	
				commands[index]=null;
			}
			String name=param[selectFirstNull(commands)];
			if((index=indexOf(Command.SALL, commands))==-1){
				fileInfo=new FileInfo(Paths.get(name));
				for(Command command:commands)
					if(command!=null)
						execute(command, fileInfo, pw);
			}else{
				commands[index]=null;
				String[] list=new File("").getAbsoluteFile().list(new DirFilter(name));
				for(String item:list){
					FileInfo fileInfo2=new FileInfo(Paths.get(item));
					for(Command command:commands)
						if(command!=null)
							execute(command, fileInfo2, pw);
				}
			}
			pw.flush();
				
		}
		pw.close();
	}
	private static int selectFirstNull(Command[] commands){
		for(int i=1;i<commands.length;i++)
			if(commands[i]==null&&commands[i-1]!=null)
				return i;
		return -1;
	}
	private static int indexOf(Command command,Command[] commands){
		for(int i=0;i<commands.length;i++)
			if(command.equals(commands[i]))
				return i;
		return -1;
	}
	private static void execute(Command command,FileInfo fileInfo,PrintWriter pw){
		switch (command) {
		case CHAR:pw.write(fileInfo.path()+", 字符数: "+fileInfo.charNum()+"\r\n");break;
		case WORD:pw.write(fileInfo.path()+", 单词数: "+fileInfo.wordNum()+"\r\n");break;
		case LINE:pw.write(fileInfo.path()+", 行   数: "+fileInfo.lineNum()+"\r\n");break;
		case ALL:fileInfo.toFile(pw);break;
		}
	}
	
	private static Command analysisCommand(String command0){
		String command =command0.replaceAll("\\s", "");
		if(command.equals("-c"))
			return Command.CHAR;
		else if(command.equals("-w"))
			return Command.WORD;
		else if(command.equals("-l"))
			return Command.LINE;
		else if(command.equals("-o"))
			return Command.OUTPUT;
		else if(command.equals("-s"))
			return Command.SALL;
		else if(command.equals("-a"))
			return Command.ALL;
		else if(command.equals("-e"))
			return Command.EXCEPT;
		else return null;
	}
}


class FileInfo{
	private int charNum;
	private int wordNum;
	private int lineSum;
	private int spaceSum;
	private int codeSum;
	private int noteSum;
	private Path path;
	private boolean isAnalysis=false;
	public FileInfo(Path path){
		this.path=path;
		charNum=0;
		wordNum=0;
		lineSum=0;
		spaceSum=0;
		codeSum=0;
		noteSum=0;
		analysis();
	}
	
	public String path(){
		return path.toString();
	}
	
	public int charNum(){
		return charNum;
	}
	public int wordNum(){
		return wordNum;
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
					switch(analysisLine(s)){
					case SPACE:spaceSum++;break;
					case CODE:codeSum++;break;
					case NOTE:noteSum++;break;
					}
					charNum+=s.replaceAll("\\s", "").length();
					wordNum+=s.split("\\W+").length;
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
	public void toFile(PrintWriter pw){
		pw.write(path.toString()+", 字符数: "+charNum+"\r\n"
				+path.toString()+", 单词数: "+wordNum+"\r\n"
				+path.toString()+", 行数: "+lineSum+"\r\n"
				+path.toString()+", 代码行/空行/注释行: "+codeSum+"/"+spaceSum+"/"+noteSum+"\r\n");
	}
	//代码行/空行/注释行/
	private Type analysisLine(String line){
		return (line.equals("")||Pattern.matches("\\s+", line))?Type.SPACE:(Pattern.matches("//.*", line)?Type.NOTE:Type.CODE);
	}
}

enum Command{
	CHAR,WORD,LINE,ALL,OUTPUT,SALL,EXCEPT;
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