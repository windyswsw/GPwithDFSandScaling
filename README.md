## Genetic Programming (GP) based resource allocation approach for VNFs:

Network Function Virtualization (NFV) is a promising technology that proposes to move network-based services from dedicated hardware middle-boxes to software running on commodity servers: Virtualized Network Functions (VNFs). As such, NFV brings the possibility of outsourcing enterprise Network Function (NFs) processing to the cloud. When an enterprise outsources its NFs to a Cloud Service Provider (CSP), the CSP is responsible for deciding: (1) where initial Virtual NFs (VNFs) should be instantiated, and (2) what, when and where additional VNFs should be instantiated to satisfy the traffic changes (scaling) with minimal impact on network performances. 

In this project we introduce a Genetic Programming (GP) based resource allocation approach for VNFs. The GP based resource allocation approach, starts with an initial solution (based on a simple Depth First Search: DFS approach) and continue with a simple scaling method that supports dynamic traffic changes over the time. The scaling approach tries to perform a vertical scaling first, and if it not successful, then it tries for a migration scaling. For the initial resource allocation, the GP approach tries to the minimize the required resources (i.e., number of servers, number of links, and average link utilization). Over the time to satisfy dynamic traffic changes (scaling), the GP approach tries to minimize required resources as well as the number of configuration changes to reduce potential service disruptions.

This repository contains programs for the GP based resource allocation algorithm. Using this data and programs, the evaluation results presented in our publications can be easily reproduced.

[1] W. Rankothge, J. Ma, F. Le, A. Russo, and J. Lobo, [“Towards making network function virtualization a cloud computing service,”] (http://repositori.upf.edu/handle/10230/26035) in IM 2015

[2] W. Rankothge, F. Le, A. Russo, and J. Lobo, [“Experimental results on the use of genetic algorithms for scaling virtualized network functions,”] (http://repositori.upf.edu/handle/10230/26036) in IEEE SDN/NFV 2015

## Project Structure:

1. Source file
2. Header files
3. Input data

## Guidelines to use the data and programs in the repository:

There are two ways that this repository can be useful for anyone looking for resource allocation approaches for VNFs.

1. Directly use the program with the given data set (in inputs folder) by running the source file

2. Use the programs with your own data sets (We have a data set generation repository at https://github.com/windyswsw/DataForNFVSDNExperiments)

## Inputs to the program:

1. Topology structure: Paths between each source and destination server pair (Paths.txt), Entering (InPaths.txt) and exiting path (OutPaths.txt) for each server
2. Topology capacities: Link capacities (Links.txt), Server capacities (Servers.txt)
3. VNF profile: Server and bandwidth requirement of VNFs (NF.txt)
4. Policy details: VNF chain information (Policy.txt)
5. Scaling requirments over the time: VNFs that needs to be scaled out/in over the time (ChangeInfo.txt)

##When using this archive, please cite the following papers:

	@INPROCEEDINGS{IM,

	AUTHOR = "W. Rankothge and J. Ma and Frank Le and A. Russo and J. Lobo",
	
	TITLE = "Towards Making Network Function Virtualization a Cloud Computing Service",
	
	BOOKTITLE = "IEEE IM 2015"}
	
	
	@INPROCEEDINGS{IEEESDN,

	AUTHOR = "W. Rankothge and Frank Le and A. Russo and J. Lobo",
	
	TITLE = "Experimental Results on the use of Genetic Algorithms for Scaling Virtualized Network Functions",
	
	BOOKTITLE = "IEEE SDN/NFV 2015"}



