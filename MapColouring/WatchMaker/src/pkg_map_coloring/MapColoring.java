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

import java.util.ArrayList;
import java.util.List;
import java.util.Random;
import org.uncommons.maths.number.NumberGenerator;
import org.uncommons.maths.random.MersenneTwisterRNG;
import org.uncommons.maths.random.Probability;
import org.uncommons.maths.random.XORShiftRNG;
import org.uncommons.watchmaker.framework.EvolutionEngine;
import org.uncommons.watchmaker.framework.EvolutionObserver;
import org.uncommons.watchmaker.framework.EvolutionaryOperator;
import org.uncommons.watchmaker.framework.GenerationalEvolutionEngine;
import org.uncommons.watchmaker.framework.PopulationData;
import org.uncommons.watchmaker.framework.factories.StringFactory;
import org.uncommons.watchmaker.framework.islands.IslandEvolution;
import org.uncommons.watchmaker.framework.islands.IslandEvolutionObserver;
import org.uncommons.watchmaker.framework.islands.RingMigration;
import org.uncommons.watchmaker.framework.operators.EvolutionPipeline;
import org.uncommons.watchmaker.framework.operators.ListCrossover;
import org.uncommons.watchmaker.framework.operators.StringCrossover;
import org.uncommons.watchmaker.framework.operators.StringMutation;
import org.uncommons.watchmaker.framework.selection.RouletteWheelSelection;
import org.uncommons.watchmaker.framework.termination.TargetFitness;
import org.uncommons.watchmaker.swing.ProbabilityParameterControl;
/**
 * Simple evolutionary algorithm that evolves a population of randomly-generated
 * strings until at least one matches a specified target string.
 * @author Daniel Dyer
 */
public final class MapColoring
{
    private static final Probability CERO_CON_CERO_UNO = new Probability(0.01d);
    public static final int cant_zonas=63;
    public static final int cant_colores=4;
     /**
     * Entry point for the sample application.  Any data specified on the
     * command line is considered to be the target String.  If no target is
     * specified, a default of "HELLOW WORLD" is used instead.
     * @param args The target String (as an array of words).
     */
    public static void main(String[] args)
    {
        List<Integer> l = evolve();
        int lugar=1;
        for(Integer i:l){
            System.out.println(lugar+" "+i);
            lugar++;
        }
    }


    public static List<Integer> evolve()
    {
        MapColoringFactory factory = new MapColoringFactory();
        List<EvolutionaryOperator<List<Integer>>> operators = new ArrayList<EvolutionaryOperator<List<Integer>>>(2);
        operators.add(new MapColoringMutation(mutColor.getNumberGenerator()));
        operators.add(new ListCrossover());
        EvolutionaryOperator<List<Integer>> pipeline = new EvolutionPipeline<List<Integer>>(operators);
        EvolutionEngine<List<Integer>> engine = new GenerationalEvolutionEngine<List<Integer>>(factory,
                                                                                 pipeline,
                                                                                 new MapEvaluator(),
                                                                                 new RouletteWheelSelection(),
                                                                                 new MersenneTwisterRNG());
        
        engine.addEvolutionObserver(new EvolutionLogger());
        return engine.evolve(100, // 100 individuals in the population.
                             1, // 5% elitism.
                             new TargetFitness(0, false));
    }

    /**
     * Trivial evolution observer for displaying information at the end
     * of each generation.
     */
     private static class EvolutionLogger implements EvolutionObserver<List<Integer>>
    {
        public void populationUpdate(PopulationData<? extends List<Integer>> data)
        {
            System.out.println("Fitness: "+data.getBestCandidateFitness());
        }
    }
     
    static ProbabilityParameterControl mutColor = new ProbabilityParameterControl(Probability.ZERO, CERO_CON_CERO_UNO, 3, new Probability(0.01d));
}
