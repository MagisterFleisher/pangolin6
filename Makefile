APP := 				pangolin0.6.1.1
#CXX := 				g++-11
CXX := 				clang++-12
# CXX :=				nvcc
BUILDDIR +=			build/
CXXFLAGS += 		$(CXXSTDFLAGS) $(CXXOPTFLAGS) $(CXXLIBFLAGS) $(CXXMACROFLAGS) $(CXXDEBUGFLAGS) $(LINKDIR)
CXXSTDFLAGS +=		-std=c++2a
CXXOPTFLAGS +=		-Ofast -march=native # -fimplicit-modules -fimplicit-module-maps -fprebuilt-module-path=. # -fprofile-use -fpermissive
CXXLIBFLAGS +=		-lstdc++ -pthread -lpthread -fopenmp -ltbb -lomp
CXXMACROFLAGS +=	-D_GLIBCXX_PARALLEL
CXXDEBUGFLAGS +=	-Wextra -Wshadow -Wnon-virtual-dtor -Wold-style-cast -Wcast-align -Wnull-dereference -Wdouble-promotion -Wformat=2 -Wduplicated-cond -Wduplicated-branches -Wuseless-cast -Wconversion -Wpessimizing-move -Wredundant-move -Wunused -Wsign-conversion # -Wpedantic 
SRCDIR += 			src/
INCLUDEDIR += 		include/
LINKDIR += 			-L/usr/include/ -Iinclude/ -L/usr/include/c++/11/
ALLFILES +=			$(SRCDIR)main.cpp $(INCLUDEDIR)Types.hpp 	$(INCLUDEDIR)GraphProperties.hpp	$(INCLUDEDIR)ReadFile.hpp	$(INCLUDEDIR)CheckFile.hpp	$(INCLUDEDIR)SimplifyGraph.hpp	$(INCLUDEDIR)DegreeCentrality.hpp	$(INCLUDEDIR)GenAlters.hpp	$(INCLUDEDIR)PrestigeSimple.hpp #$(INCLUDEDIR)CSV.hpp 	$(INCLUDEDIR)GenNodelist.hpp	$(INCLUDEDIR)TriadGen.hpp	$(INCLUDEDIR)GenCliques.hpp $(INCLUDEDIR)GenEdgemap.hpp	$(INCLUDEDIR)GenNodeEdgemap.hpp
ALLFILESO +=		$(BUILDDIR)main.o $(BUILDDIR)Types.o 		$(BUILDDIR)GraphProperties.o		$(BUILDDIR)ReadFile.o		$(BUILDDIR)CheckFile.o 		$(BUILDDIR)SimplifyGraph.o		$(BUILDDIR)DegreeCentrality.o		$(BUILDDIR)GenAlters.o  	$(BUILDDIR)PrestigeSimple.o		#$(BUILDDIR)CSV.o 		$(BUILDDIR)GenNodelist.o					$(BUILDDIR)TriadGen.o		$(BUILDDIR)GenCliques.o		$(BUILDDIR)GenEdgemap.o		$(BUILDDIR)GenNodeEdgemap.o

$(APP): $(ALLFILESO)
	$(CXX) $(CXXFLAGS) $(BUILDDIR)main.o -o $(APP)

$(BUILDDIR)main.o: $(ALLFILES)
	$(CXX) $(CXXFLAGS) -c $(SRCDIR)main.cpp -o $(BUILDDIR)main.o

$(BUILDDIR)Types.o: $(INCLUDEDIR)Types.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)Types.hpp -o $(BUILDDIR)Types.o

$(BUILDDIR)GraphProperties.o: $(INCLUDEDIR)GraphProperties.hpp $(INCLUDEDIR)Types.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)GraphProperties.hpp -o $(BUILDDIR)GraphProperties.o

$(BUILDDIR)ReadFile.o: $(INCLUDEDIR)ReadFile.hpp $(INCLUDEDIR)Types.hpp  $(INCLUDEDIR)GraphProperties.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)ReadFile.hpp -o $(BUILDDIR)ReadFile.o

$(BUILDDIR)CheckFile.o: $(INCLUDEDIR)CheckFile.hpp $(INCLUDEDIR)Types.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)CheckFile.hpp -o $(BUILDDIR)CheckFile.o

$(BUILDDIR)SimplifyGraph.o: $(INCLUDEDIR)SimplifyGraph.hpp $(INCLUDEDIR)Types.hpp $(INCLUDEDIR)ReadFile.hpp $(INCLUDEDIR)GraphProperties.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)SimplifyGraph.hpp -o $(BUILDDIR)SimplifyGraph.o

$(BUILDDIR)DegreeCentrality.o: $(INCLUDEDIR)DegreeCentrality.hpp $(INCLUDEDIR)Types.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)DegreeCentrality.hpp -o $(BUILDDIR)DegreeCentrality.o

$(BUILDDIR)GenAlters.o: $(INCLUDEDIR)GenAlters.hpp $(INCLUDEDIR)Types.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)GenAlters.hpp -o $(BUILDDIR)GenAlters.o

$(BUILDDIR)PrestigeSimple.o: $(INCLUDEDIR)PrestigeSimple.hpp $(INCLUDEDIR)Types.hpp
	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)PrestigeSimple.hpp -o $(BUILDDIR)PrestigeSimple.o

# $(BUILDDIR)CSV.o: $(INCLUDEDIR)CSV.hpp $(INCLUDEDIR)CheckFile.hpp $(INCLUDEDIR)Types.hpp
# 	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)CSV.hpp -o $(BUILDDIR)CSV.o

# $(BUILDDIR)GenNodelist.o: $(INCLUDEDIR)GenNodelist.hpp $(INCLUDEDIR)Types.hpp
# 	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)GenNodelist.hpp -o $(BUILDDIR)GenNodelist.o


# $(BUILDDIR)GenCliques.o: $(INCLUDEDIR)GenCliques.hpp $(INCLUDEDIR)Types.hpp $(INCLUDEDIR)GenAlters.hpp
# 	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)GenCliques.hpp -o $(BUILDDIR)GenCliques.o

# $(BUILDDIR)TriadGen.o: $(INCLUDEDIR)TriadGen.hpp $(INCLUDEDIR)Types.hpp
# 	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)TriadGen.hpp -o $(BUILDDIR)TriadGen.o


# $(BUILDDIR)GenEdgemap.o: $(INCLUDEDIR)GenEdgemap.hpp $(INCLUDEDIR)Types.hpp
# 	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)GenEdgemap.hpp -o $(BUILDDIR)GenEdgemap.o

# $(BUILDDIR)GenEdgemap.o: $(INCLUDEDIR)GenNodeEdgemap.hpp $(INCLUDEDIR)Types.hpp
# 	$(CXX) $(CXXFLAGS) -c $(INCLUDEDIR)GenNodeEdgemap.hpp -o $(BUILDDIR)GenNodeEdgemap.o