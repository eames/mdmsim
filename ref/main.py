from pyspark import StorageLevel, SparkFiles, SparkContext, SparkConf
import subprocess,time,json

def run_sim(index) :
  with open("config_22Ne_6Li_geant.json") as infile:
    config = json.load(infile)
  config["processNumber"] = index
  with open("config_22Ne_6Li_geant_{0}.json",format(index),"w") as outfile :
    json.dump(config,outfile,indent = 4)
  subprocess.call(["./mdm","{0}".format(index)])
  subprocess.call(["mv","MDM_{0}.root".format(index),"/hdfs/user/hjayatissa/geant_inelastic_22ne_6li"])
  subprocess.call(["/usr/local/hadoop/bin/hdfs","dfs","-chown","hjayatissa","/user/hjayatissa/geant_inelastic_22ne_6li/MDM_{0}.root".format(index)])

if __name__ == "__main__" :
  sconf = SparkConf().setAppName("inelastic_22ne_6li_sim")
  sc = SparkContext(conf=sconf)
    
  sc.addFile("mdm")
  sc.addFile("run.mac")
  sc.addFile("config_22Ne_6Li_geant.json");

  distData = sc.parallelize(range(0,20),20)

  distData.foreach(lambda x: run_sim(x))

