def calc_next_position(self, err: list, manipPos: list):
    self.logger.info(f"Errors: {err}")
    self.logger.info(f"CUR_POS: {manipPos}")
    c_0_x = -0.0140625 * err[0]
    c_1_y = -0.026041667 * err[1]
    c_1_z = -0.00375 * err[2]
    c_2_y = -0.026041667 * err[1]
    c_2_z = 0.00225 * err[2]

    newPos = [
        int(manipPos[0] + c_0_x),
        int(manipPos[1] + c_1_y + c_1_z),
        int(manipPos[2] + c_2_y + c_2_z),
        GRIP_OPENED
    ]
    self.logger.info(f"NEW_POS: {newPos}")
    return newPos
