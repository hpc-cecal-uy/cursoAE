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

import java.awt.Color;
import java.awt.Dimension;
import java.awt.Point;
import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import org.uncommons.watchmaker.framework.factories.AbstractCandidateFactory;

/**
 * Creates random polygon-based images.
 * @author Daniel Dyer
 */
public class MapColoringFactory extends AbstractCandidateFactory<List<Integer>>
{
    public MapColoringFactory()
    {
    }

    public List<Integer> generateRandomCandidate(Random rng)
    {
       List<Integer> candidato= new ArrayList<Integer>();
       for(int i=0;i<MapColoring.cant_zonas;i++){
           int zona=i+1;
           int color=(new Random().nextInt(MapColoring.cant_colores)+1);
           candidato.add(color);
       }
       return candidato;
    }
}
