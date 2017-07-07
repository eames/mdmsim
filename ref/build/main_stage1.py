from pyspark import StorageLevel, SparkFiles, SparkContext, SparkConf
import subprocess,time,json

def run_sim(index) :
  with open("config_isobutane_22Ne_6Li_geant.json") as infile :
    config = json.load(infile)
  config["processNumber"] = index
  with open("config_{0}.json".format(index),"w") as outfile :
    json.dump(config,outfile,indent = 4)
  subprocess.call(["./mdm","config_{0}.json".format(index)])
  subprocess.call(["mv","MDM_{0}.root".format(index),"/hdfs/user/hjayatissa/geant_mdm_csi/stage1"])
  subprocess.call(["/usr/local/hadoop/bin/hdfs","dfs","-chown","hjayatissa","/user/hjayatissa/geant_mdm_csi/stage1/MDM_{0}.root".format(index)])

if __name__ == "__main__" :
  sconf = SparkConf().setAppName("mdm-CsI")
  sc = SparkContext(conf=sconf)
    
  sc.addFile("mdm")
  sc.addFile("config/config_isobutane_22Ne_6Li_geant.json")
  sc.addFile("run.mac")

  distData = sc.parallelize(range(0,100),70)

  distData.foreach(lambda x: run_sim(x))

