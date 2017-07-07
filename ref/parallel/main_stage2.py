from pyspark import StorageLevel, SparkFiles, SparkContext, SparkConf
import subprocess,time,json,re

def stage2(fileuri) :
  filepath="/hdfs"+fileuri[1][31:]
  print filepath
  with open("config_isobutane_22Ne_6Li_geant_oxford.json") as infile :
    config = json.load(infile)
  search = re.search("^.*MDM_(.*)\\.root$",filepath)
  index = int(search.group(1));
  config["processNumber"] = index;
  config["stage1File"] = filepath
  with open("config_{0}.json".format(index),"w") as outfile :
    json.dump(config,outfile,indent = 4)
  subprocess.call(["./mdm","config_{0}.json".format(index)])
  subprocess.call(["mv","Final_MDM_{0}.root".format(index),"/hdfs/user/hjayatissa/geant_mdm_csi/stage2"])
  subprocess.call(["/usr/local/hadoop/bin/hdfs","dfs","-chown","hjayatissa","/user/hjayatissa/geant_mdm_csi/stage2/Final_MDM_{0}.root".format(index)])

if __name__ == "__main__" :
  sconf = SparkConf().setAppName("mdm-CsI-2")
  sconf.set("spark.executor.memory","13g")
  sconf.set("spark.python.worker.reuse","false")
  sc = SparkContext(conf=sconf)
    
  sc.addFile("mdm")
  sc.addFile("config/config_isobutane_22Ne_6Li_geant_oxford.json")
  sc.addFile("run_oxf.mac")

  file_name = "hdfs://gr-gmaster.tamu.edu:9000//user/hjayatissa/geant_mdm_csi/stage1/MDM_*.root"
  lines = sc.newAPIHadoopFile(file_name,"edu.tamu.hadoop.RootInputFormat",
      "org.apache.hadoop.io.IntWritable","org.apache.hadoop.io.Text")
  lines.foreach(lambda x : stage2(x))
