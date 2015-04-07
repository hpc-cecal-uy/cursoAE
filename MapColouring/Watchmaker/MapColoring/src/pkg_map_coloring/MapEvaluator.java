//=============================================================================
// Copyright 2006-2010 Daniel W. Dyer
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//=============================================================================
package pkg_map_coloring;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileNotFoundException;
import java.io.FileReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.nio.charset.StandardCharsets;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.ArrayList;
import java.util.List;
import java.util.logging.Level;
import java.util.logging.Logger;
//import java.util.stream.Stream;
import org.uncommons.watchmaker.framework.FitnessEvaluator;


public class MapEvaluator implements FitnessEvaluator<List<Integer>>
{
    static List<List<Integer>> ady= new ArrayList<>();
    static{
        for(int i=0;i<MapColoring.cant_zonas; i++)
            ady.add(null);
    }

    public MapEvaluator()
    {
        
        // Open the file
        FileInputStream fstream=null;
        try {
            
            fstream = new FileInputStream("limite_barrios_separado_por_guiones.csv");
        
            BufferedReader br = new BufferedReader(new InputStreamReader(fstream));
           
            //Read File Line By Line
            String todo=br.readLine();
            String[] lines= todo.split("-");
            for (String strLine : lines)   {
              List<Integer> l= new ArrayList<Integer>();
              String[] arrS= strLine.split(",");
              for(String s:arrS){
                  l.add(Integer.parseInt(s));
              }
              int lugar= l.get(0)-1;//obtengo el id del lugar
              l.remove(0);//saco el lugar de sus propias adyacencias
              ady.remove(lugar);//agrego l como lista de adyacencias
              ady.add(lugar, l);//del lugar correspondiente
            }

            //Close the input stream
            br.close();
        } catch (Exception ex) {
            Logger.getLogger(MapEvaluator.class.getName()).log(Level.SEVERE, null, ex);
        }
    }

    public double getFitness(List<Integer> ind,
                             List<? extends List<Integer>> population)
    {
        int errors = 0;
        int lugar=0;
        for(List<Integer> l:ady){
            for(Integer i:l){
                if(ind.get(i-1)==ind.get(lugar))
                    errors++;
            }
            lugar++;
        }
        return errors;
    }


    public boolean isNatural()
    {
        return false;
    }
}
