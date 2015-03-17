package ec.app.mapcolouring;
import ec.*;
import ec.simple.*;
import ec.vector.*;

public class Mapcolouring extends Problem implements SimpleProblemForm
    {
    public void evaluate(final EvolutionState state,
        final Individual ind,
        final int subpopulation,
        final int threadnum)


        {
        if (ind.evaluated) return;

        if (!(ind instanceof IntegerVectorIndividual))
            state.output.fatal("Error. No es un vector de enteros!",null);
        
        IntegerVectorIndividual ind2 = (IntegerVectorIndividual)ind;
        IntegerVectorSpecies t_spe = (IntegerVectorSpecies)ind2.species;

        double fitness=0;
        int [][] limites = t_spe.getLimitesBarrios();

        //CALCULO EL FITNESS
        for (int i=0;i<ind2.genome.length;i++){
        	for (int j=0; j<limites[i].length;j++){
        		if (ind2.genome[limites[i][j]-1]==ind2.genome[i])
        			fitness+=1;
        	}
        }

        //Asigno el fitness al individuo
        if (!(ind2.fitness instanceof SimpleFitness))
            state.output.fatal("Error. No es un SimpleFitness",null);
        ((SimpleFitness)ind2.fitness).setFitness(state,fitness*(-1), fitness == 0);
        ind2.evaluated = true;
        }
    }
