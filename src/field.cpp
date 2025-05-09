#include "field.h"
namespace Femora{



UniformField<real> divergence(const UniformField<vector2d<real>>& vecField){
    Femora::UniformField<real> result(
        vecField.getWidth(), 
        vecField.getHeight(), 
        vecField.getDx(),
        0);
    
        for(size_t i = 1; i < vecField.getHeight() - 1; i++){
            for(size_t j = 1; j < vecField.getWidth() - 1; j++){
                
                result.setValue(i, j, (
                    vecField.getValue(i+1, j).y - vecField.getValue(i-1, j).y +
                    vecField.getValue(i, j+1).x - vecField.getValue(i, j-1).x
                ) / (2.0 * vecField.getDx()));

            }
        }
        return result;
        
}
UniformField<real> curl_2d(const UniformField<vector2d<real>>& vecField){
    Femora::UniformField<real> result(
        vecField.getWidth(), 
        vecField.getHeight(), 
        vecField.getDx(),
        0);
    
        for(size_t i = 1; i < vecField.getHeight() - 1; i++){
            for(size_t j = 1; j < vecField.getWidth() - 1; j++){
                //vecField的y值对x求导 - vecField的x值对y求导
                result.setValue(i, j, (
                    vecField.getValue(i, j+1).y - vecField.getValue(i, j-1).y -
                    vecField.getValue(i+1, j).x + vecField.getValue(i-1, j).x 
                ) / (2.0 * vecField.getDx()));

            }
        }
        return result;
        
}




}
