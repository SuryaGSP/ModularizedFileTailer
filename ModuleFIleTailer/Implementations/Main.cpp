#include "../HeaderFiles/Includes.h"
#include "../HeaderFiles/ProcessorQueue.h"
#include "../HeaderFiles/DBOperations.h"
#include "../HeaderFiles/DirectoryMonitor.h"
#include "../HeaderFiles/FileInfo.h"
void IntializeThreadPoolInFileProcessor() {
  std::thread t1[noOfThreads];
  for (int iter = 0; iter < 4; iter++)
  {
    t1[iter] = std::thread(ProcessorQueue::WorkerThreads, iter);
  }
  for (std::thread &t : t1)
  {
    t.join();
  }
}
int main()
{
  DBOperations::OpenConnection();
  std::thread tinit(IntializeThreadPoolInFileProcessor);
  std::string fileInput = "[{ \"locations\": [{ \"include\": \"D:\\\\TestDir\\\\\" }, { \"include\": \"D:\\\\TestDir2\\\\\", \"exclude\": [\"D:\\\\TestDir2\\\\archive\\\\\"], \"includetypes\": [\".log\"] }, { \"include\": \"D:\\\\App1\\\\\", \"exclude\": [\"D:\\\\App1\\\\*\"], \"excludetypes\": [\".dmp\", \".tmp\"] }, { \"include\": \"C:\\\\Users\\\\surya-pt2591\\\\Downloads\\\\apache-tomcat-8.5.37-windows-x64\\\\apache-tomcat-8.5.37\\\\logs\\\\\", \"pattern\": \"localhost_access_log.${yyyy}-${MM}-${dd}\" }] }]";
  ProcessJSON(fileInput);
  FileInfo::UpdateDB();
  FileInfo::PrintFileNamesMonitored();
  int x = 10;
  std::vector<DirectoryMonitor> dirMoniObjects(x);
  DirectoryMonitor::TotalDirectories = FileInfo::CountMonitorDirecs();
  DirectoryMonitor::LoadMonitorDirecs();
  int i = 0;
  for (std::string &s : DirectoryMonitor::monitorDirecs)
  {
    DirectoryMonitor * d = new DirectoryMonitor;
    d->SetDirName(s);
    d->Createhandle();
    d->SetOverlap();
    d->setDirDetails(FileInfo::dirIncludeTypeMap[s], FileInfo::dirExcludeTypeMap[s], FileInfo::dirPatternMap[s]);
    dirMoniObjects[i++] = *d;
  }
  std::thread threadForMonitoring(DirectoryMonitor::CreateThreadForMonitoringDirectory, dirMoniObjects);
  std::cin.get();
  return 0;
}